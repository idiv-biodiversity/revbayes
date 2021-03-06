#include "DagNode.h"
#include "FileMonitor.h"
#include "MonteCarloSampler.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbIterator.h"
#include "RbMathLogic.h"
#include "RbOptions.h"
#include "RlUserInterface.h"
#include "SequenctialMoveSchedule.h"
#include "SingleRandomMoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "ExtendedNewickTreeMonitor.h"

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
MonteCarloSampler::MonteCarloSampler(void) :
    generation(0)
{

}


/**
 * Destructor. Nothing to do here.
 */
MonteCarloSampler::~MonteCarloSampler(void)
{
    
}


/**
 * Get the current generation number.
 */
size_t MonteCarloSampler::getCurrentGeneration( void ) const
{
    return generation;
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const MonteCarloSampler& x)
{
    o << "MonteCarloSampler";
    
    return o;
}



