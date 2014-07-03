#include "DagNode.h"
#include "FileMonitor.h"
#include "MonteCarloSampler.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"
#include "SingleRandomMoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "ExtendedNewickTreeMonitor.h"
#include "ExtendedNewickAdmixtureTreeMonitor.h"
#include "AdmixtureBipartitionMonitor.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <typeinfo>

using namespace RevBayesCore;


/**
 * Constructor. We create an independent copy of the model and thus of all DAG nodes.
 * Someone might have wanted to run another MonteCarloSampler with different settings on the same model.
 * Thus we also create our own copies of the monitors and moves.
 *
 * \param[in]    m    The model containing all DAG nodes.
 * \param[in]    mvs  The vector of moves.
 * \param[in]    mons The vector of monitors.
 */
MonteCarloSampler::MonteCarloSampler(const Model& m, const std::vector<Move*> &mvs, const std::vector<Monitor*> &mons) :
    chainActive( true ),
    chainHeat( 1.0 ),
    chainIdx( 0 ),
    generation(0),
    model( m ),
    monitors(),
    moves(),
    schedule(NULL),
    scheduleType("random")
{
    
    // create an independent copy of the model, monitors and moves
    replaceDag(mvs,mons);
    
    initializeChain();
    initializeMonitors();
    
}

/**
 * Copy constructor. For more details see the constructor.
 *
 * \param[in]    m    The MonteCarloSampler object to copy.
 */
MonteCarloSampler::MonteCarloSampler(const MonteCarloSampler &m) :
    chainActive(m.chainActive),
    chainHeat(m.chainHeat),
    chainIdx(m.chainIdx),
    generation(m.generation),
    model( m.model ),
    monitors(),
    moves(),
    schedule(NULL),
    scheduleType( m.scheduleType )
{
    
    // temporary references
    const std::vector<Monitor*>& mons = m.monitors;
    const std::vector<Move*>& mvs = m.moves;
    
    
    // create an independent copy of the model, monitors and moves
    replaceDag(mvs,mons);
    
    initializeChain();
    initializeMonitors();
}


/**
 * Destructor. Frees the DAG nodes (the model), moves, monitor and the move schedule.
 */
MonteCarloSampler::~MonteCarloSampler(void) {
    
    // free the moves and monitors
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it)
    {
        Move *theMove = (*it);
        delete theMove;
    }
    
    for (std::vector<Monitor*>::iterator it = monitors.begin(); it != monitors.end(); ++it)
    {
        Monitor *theMonitor = (*it);
        delete theMonitor;
    }
    
    // delete the move schedule
    delete schedule;
}


void MonteCarloSampler::burnin(size_t generations, size_t tuningInterval) {
    
    // Initialize objects needed by chain
    initializeChain();
    initializeMonitors();
    
    // reset the counters for the move schedules
    double movesPerIteration = 0.0;
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it)
    {
        (*it)->resetCounters();
        movesPerIteration += (*it)->getUpdateWeight();
    }
    
    std::cout << "Running Monte Carlo Sampler while performing " << movesPerIteration << " proposals per iteration." << std::endl;
    
    size_t printInterval = size_t( round( fmax(1,generations/20.0) ) );
    
    if (chainActive)
    {
        std::cout << "burning in the chain ..." << std::endl;
        std::cout << "0--------25--------50--------75--------100" << std::endl;
        std::cout << "*";
        std::cout.flush();
    }
    
    // Run the chain
    for (size_t k=1; k<=generations; k++)
    {
        
        if ( k % printInterval == 0 )
        {
            std::cout << "**";
            std::cout.flush();
        }
        
        nextCycle(false);
        
        // check for autotuning
        if ( k % tuningInterval == 0 )
        {
            
            // tune the moves
            for (size_t i=0; i<moves.size(); i++)
            {
                moves[i]->autoTune();
            }
        }
        
    }
    
    if (chainActive)
    {
        std::cout << std::endl;
    }
}



double MonteCarloSampler::getChainHeat(void)
{
    return chainHeat;
}

size_t MonteCarloSampler::getChainIndex(void)
{
    return chainIdx;
}


double MonteCarloSampler::getLnPosterior(void)
{
    return lnProbability;
}

double MonteCarloSampler::getModelLnProbability(void)
{
    const std::vector<DagNode*> &n = model.getDagNodes();
    double pp = 0.0;
    for (std::vector<DagNode*>::const_iterator it = n.begin(); it != n.end(); ++it) {
        //std::cout << (*it)->getName() << "  " << (*it)->getLnProbability() << "\n";
        //(*it)->touch();
        pp += (*it)->getLnProbability();
    }
    return pp;
}


std::vector<Monitor*>& MonteCarloSampler::getMonitors(void)
{
    return monitors;
}



/** Creates a vector of stochastic nodes, starting from the source nodes to the sink nodes */
void MonteCarloSampler::getOrderedStochasticNodes(const DagNode* dagNode,  std::vector<DagNode*>& orderedStochasticNodes, std::set<const DagNode*>& visitedNodes) {
    
    if (visitedNodes.find(dagNode) != visitedNodes.end()) { //The node has been visited before
                                                            //we do nothing
        return;
    }
    
    // add myself here for safety reasons
    visitedNodes.insert( dagNode );
    
    if ( dagNode->isConstant() ) { //if the node is constant: no parents to visit
        std::set<DagNode*> children = dagNode->getChildren() ;
        visitedNodes.insert(dagNode);
        std::set<DagNode*>::iterator it;
        for ( it = children.begin() ; it != children.end(); it++ )
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
    }
    else //if the node is stochastic or deterministic
    {
        // First I have to visit my parents
        const std::set<const DagNode *>& parents = dagNode->getParents() ;
        std::set<const DagNode *>::const_iterator it;
        for ( it=parents.begin() ; it != parents.end(); it++ )
            getOrderedStochasticNodes(*it, orderedStochasticNodes, visitedNodes);
        
        // Then I can add myself to the nodes visited, and to the ordered vector of stochastic nodes
        //        visitedNodes.insert(dagNode);
        if ( dagNode->isStochastic() ) //if the node is stochastic
            orderedStochasticNodes.push_back( const_cast<DagNode*>( dagNode ) );
        
        // Finally I will visit my children
        std::set<DagNode*> children = dagNode->getChildren() ;
        std::set<DagNode*>::iterator it2;
        for ( it2 = children.begin() ; it2 != children.end(); it2++ )
            getOrderedStochasticNodes(*it2, orderedStochasticNodes, visitedNodes);
    }
    
    return;
}

void MonteCarloSampler::initializeChain( void ) {
    
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
    std::vector<DagNode *> orderedStochNodes;
    std::set< const DagNode *> visited;
    getOrderedStochasticNodes(dagNodes[0],orderedStochNodes, visited );
    
    /* Get rid of previous move schedule, if any */
    if ( schedule )
    {
        delete schedule;
    }
    schedule = NULL;
    
    /* Get initial lnProbability of model */
    
    // first we touch all nodes so that the likelihood is dirty
    for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++)
    {
        (*i)->touch();
    }
    
    
    // redraw parameters for inactive chains in pMC^3 team
    if (chainActive == false)
    {
        for (std::vector<DagNode *>::iterator i=orderedStochNodes.begin(); i!=orderedStochNodes.end(); i++)
        {
         
            if ( !(*i)->isClamped() && (*i)->isStochastic() )
            {
            
                (*i)->redraw();
                
            }
            else if ( (*i)->isClamped() )
            {
                // make sure that the clamped node also recompute their probabilities
                (*i)->touch();
            }
            
        }
    }
    
    int numTries    = 0;
    int maxNumTries = 100;
    for ( ; numTries < maxNumTries; numTries ++ )
    {
        lnProbability = 0.0;
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++)
        {
            DagNode* node = (*i);
            (*i)->touch();
            
            double lnProb = node->getLnProbability();
            
            if ( !RbMath::isAComputableNumber(lnProb) )
            {
                std::cerr << "Could not compute lnProb for node " << node->getName() << "." << std::endl;
                node->printValue(std::cerr,"");
                std::cerr << std::endl;
            }
            lnProbability += lnProb;
            
        }
        
        // now we keep all nodes so that the likelihood is stored
        for (std::vector<DagNode *>::iterator i=dagNodes.begin(); i!=dagNodes.end(); i++)
        {
            (*i)->keep();
        }
        
        if ( !RbMath::isAComputableNumber( lnProbability ) )
        {
            std::cerr << "Drawing new initial states ... " << std::endl;
            for (std::vector<DagNode *>::iterator i=orderedStochNodes.begin(); i!=orderedStochNodes.end(); i++)
            {
                
                if ( !(*i)->isClamped() && (*i)->isStochastic() )
                {
                    (*i)->redraw();
                    
                }
                else if ( (*i)->isClamped() )
                {
                    // make sure that the clamped node also recompute their probabilities
                    (*i)->reInitialized();
                    (*i)->touch();
                }
                
            }
        }
        else
        {
            break;
        }
        
    }
    
    if ( numTries == maxNumTries )
    {
        std::stringstream msg;
        msg << "Unable to find a starting state with computable probability";
        if ( numTries > 1 )
            msg << " after " << numTries << " tries";
        throw RbException( msg.str() );
        
    }
    
    /* Create the move scheduler */
    if ( scheduleType == "sequential" )
    {
        schedule = new SequentialMoveSchedule( moves );
    }
    else if ( scheduleType == "single" )
    {
        schedule = new SingleRandomMoveSchedule( moves );
    }
    else
    {
        schedule = new RandomMoveSchedule( moves );
    }
    
    generation = 0;
}



void MonteCarloSampler::initializeMonitors(void)
{
    for (size_t i=0; i<monitors.size(); i++)
    {
        monitors[i]->setModel( &model );
    }
}



bool MonteCarloSampler::isChainActive(void)
{
    return chainActive;
}

void MonteCarloSampler::monitor(unsigned long g)
{
    
    // Monitor
    for (size_t i = 0; i < monitors.size(); i++)
    {
        monitors[i]->monitor( g );
    }
    
}



unsigned long MonteCarloSampler::nextCycle(bool advanceCycle) {
    
#ifdef DEBUG_MCMC
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
#endif
    
    size_t proposals = size_t( round( schedule->getNumberMovesPerIteration() ) );
    for (size_t i=0; i<proposals; i++)
    {
#ifdef DEBUG_MCMC
        double oldLnProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            oldLnProb += (*it)->getLnProbability();
        }
#endif
        
        // Get the move
        Move* theMove = schedule->nextMove( generation );
        theMove->perform( chainHeat, false);
        
#ifdef DEBUG_MCMC
        double lnProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            lnProb += (*it)->getLnProbability();
        }
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            (*it)->touch();
        }
        double touchedLnProb = 0.0;
        double lnLikelihoodProb = 0.0;
        double lnPriorProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            touchedLnProb += (*it)->getLnProbability();
            if ( (*it)->isClamped() )
            {
                lnLikelihoodProb += (*it)->getLnProbability();
            }
            else
            {
                lnPriorProb += (*it)->getLnProbability();
            }
        }
        
        if ( fabs(lnProb - touchedLnProb) > 1E-6 )
        {
            std::cout << "Failure occurred after move:\t" << theMove->getMoveName() << std::endl;
            throw RbException("Error in MonteCarloSampler probability computation.");
        }
#endif
        
    }
    
    
    // advance gen cycle if needed (i.e. run()==true, burnin()==false)
    if (advanceCycle)
        generation++;
    
    // gen number used for p(MC)^3
    return generation;
}


void MonteCarloSampler::printOperatorSummary(void) const {
    
    
    // printing the moves summary
    std::cerr << std::endl;
    std::cerr << "                  Name                  | Param              |  Weight  |  Tried   | Accepted | Acc. Ratio| Parameters" << std::endl;
    std::cerr << "===============================================================================================================================" << std::endl;
    for (std::vector<Move*>::const_iterator it = moves.begin(); it != moves.end(); ++it)
    {
        (*it)->printSummary(std::cerr);
    }
    
    std::cout << std::endl;
}



void MonteCarloSampler::replaceDag(const std::vector<Move *> &mvs, const std::vector<Monitor *> &mons)
{
    
    // we need to replace the DAG nodes of the monitors and moves
    const std::vector<DagNode*>& modelNodes = model.getDagNodes();
    for (std::vector<Move*>::const_iterator it = mvs.begin(); it != mvs.end(); ++it) {
        Move *theMove = (*it)->clone();
        std::set<DagNode*> nodes = theMove->getDagNodes();
        for (std::set<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j) {
            
            // error checking
            if ( (*j)->getName() == "" )
                throw RbException( "Unable to connect move to DAG copy because variable name was lost");
            
            DagNode* theNewNode = NULL;
            for (std::vector<DagNode*>::const_iterator k = modelNodes.begin(); k != modelNodes.end(); ++k) {
                if ( (*k)->getName() == (*j)->getName() ) {
                    theNewNode = *k;
                    break;
                }
            }
            // error checking
            if ( theNewNode == NULL ) {
                throw RbException("Cannot find node with name '" + (*j)->getName() + "' in the model but received a move working on it.");
            }
            
            // now swap the node
            theMove->swapNode( *j, theNewNode );
        }
        moves.push_back( theMove );
    }
    
    for (std::vector<Monitor*>::const_iterator it = mons.begin(); it != mons.end(); ++it)
    {
        Monitor *theMonitor = (*it)->clone();
        std::vector<DagNode*> nodes = theMonitor->getDagNodes();
        for (std::vector<DagNode*>::const_iterator j = nodes.begin(); j != nodes.end(); ++j)
        {
            
            // error checking
            if ( (*j)->getName() == "" )
                throw RbException( "Unable to connect monitor to DAG copy because variable name was lost");
            
            DagNode* theNewNode = NULL;
            for (std::vector<DagNode*>::const_iterator k = modelNodes.begin(); k != modelNodes.end(); ++k)
            {
                if ( (*k)->getName() == (*j)->getName() )
                {
                    theNewNode = *k;
                    break;
                }
            }
            // error checking
            if ( theNewNode == NULL )
            {
                throw RbException("Cannot find node with name '" + (*j)->getName() + "' in the model but received a monitor working on it.");
            }
            
            // now swap the node
            theMonitor->swapNode( *j, theNewNode );
        }
        monitors.push_back( theMonitor );
    }
}


void MonteCarloSampler::run(size_t kIterations) {
    
    /* Let user know what we are doing */
    if ( generation == 0 )
        std::cout << "Running MonteCarloSampler simulation for " << kIterations << " iterations" << std::endl;
    else
        std::cout << "Appending " << kIterations << " iterations to previous MonteCarloSampler simulation of " << generation << " iterations" << std::endl;
    
    if ( scheduleType == "single" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves, with a" << std::endl;
        std::cout << "single move picked randomly per iteration" << std::endl;
    }
    else if ( scheduleType == "random" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves in a random" << std::endl;
        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    else if ( scheduleType == "sequential" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves in a sequential" << std::endl;
        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    
    // Initialize objects used in run
    initializeChain();
    initializeMonitors();
    
    if ( generation == 0 )
    {
        // Monitor
        startMonitors();
        monitor(0);
    }
    
    // reset the counters for the move schedules
    for (std::vector<Move*>::iterator it = moves.begin(); it != moves.end(); ++it)
    {
        (*it)->resetCounters();
    }
    
    // Run the chain
    for (size_t k=1; k<=kIterations; k++)
    {
        nextCycle(true);
        
        // Monitor
        monitor(generation);
        
    }
    
    
}




void MonteCarloSampler::setChainActive(bool tf)
{
    chainActive = tf;
}

void MonteCarloSampler::setChainHeat(double v)
{
    chainHeat = v;
}

void MonteCarloSampler::setChainIndex(size_t x)
{
    chainIdx = x;
}


void MonteCarloSampler::setScheduleType(const std::string &s)
{
    
    scheduleType = s;
}


void MonteCarloSampler::startMonitors( void ) {
    
    /* Open the output file and print headers */
    for (size_t i=0; i<monitors.size(); i++)
    {
        
        // open filestream for each monitor
        //monitors[i]->openStream();
        
        // if this chain is active, print the header
        if (chainActive) // surprised this works properly...
        {
            monitors[i]->openStream();
            monitors[i]->printHeader();
            
        }
    }
}
