/**
 * @file
 * This file contains the Workspace function that adds types and functions
 * to the global workspace, registering them with the interpreter/compiler
 * during the process.
 *
 * @brief Function registering language objects
 *
 * Instructions
 *
 * This is the central registry of Rev objects. It is a large file and needs
 * to be properly organized to facilitate maintenance. Follow these simple
 * guidelines to ensure that your additions follow the existing structure.
 * 
 * 1. All headers are added in groups corresponding to directories in the
 *    revlanguage code base.
 * 2. All objects (types, distributions, and functions) are registered in
 *    groups corresponding to directories in the revlanguage code base.
 * 3. All entries in each group are listed in alphabetical order.
 *
 * Some explanation of the directory structure is provided in the comments
 * in this file. Consult these comments if you are uncertain about where
 * to add your objects in the code.
 *
 * (c) Copyright 2009-
 * @license GPL version 3
 *
 */

#include <sstream>
#include <vector>
#include <set>
#include <cstdlib>

/* Files including helper classes */
#include "RbException.h"
#include "RlUserInterface.h"
#include "Workspace.h"

/// Miscellaneous types ///

/* Primitive types (in folder "datatypes/basic") */
#include "Complex.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"

/* Container types (in folder "datatypes/container") */
#include "Vector.h"

/* Evolution types (in folder "datatypes/evolution") */

/* Character state types (in folder "datatypes/evolution/character") */
#include "RlAminoAcidState.h"
#include "RlDnaState.h"
#include "RlRnaState.h"

/* Character data types (in folder "datatypes/evolution/datamatrix") */

/* Tree types (in folder "datatypes/evolution/trees") */
#include "RlClade.h"

/* Inference types (in folder "datatypes/inference") */
#include "RlBurninEstimationConvergenceAssessment.h"
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlParallelMcmcmc.h"
#include "RlPathSampler.h"
#include "RlPowerPosterior.h"
#include "RlSteppingStoneSampler.h"

/// Monitors ///

/* Monitor types (in folder "datatypes/inference/monitors) */
#include "RlMonitor.h"
#include "Mntr_File.h"
#include "Mntr_ExtendedNewickFile.h"
#include "Mntr_Model.h"
#include "Mntr_Screen.h"

/// Moves ///

/* Move types (in folder "datatypes/inference/moves") (grouped by parameter type) */
#include "RlMove.h"

/* Moves on real values */
#include "Move_Scale.h"
#include "Move_Slide.h"

/* Moves on integer values */
#include "Move_RandomGeometricWalk.h"
#include "Move_RandomIntegerWalk.h"

/* Moves on simplices */
#include "Move_Simplex.h"
#include "Move_SimplexSingleElementScale.h"

/* Moves on real valued vectors */
#include "Move_RLCRateScale.h"
#include "Move_SingleElementScale.h"
#include "Move_SwitchRateJump.h"
#include "Move_VectorSingleElementScale.h"
#include "Move_VectorSingleElementSlide.h"
#include "Move_VectorScale.h"

/* Moves on precision matrices */
#include "Move_VectorSingleElementSlide.h"

/* Tree proposals (in folder "datatypes/inference/moves/tree") */
#include "Move_FNPR.h"
#include "Move_NarrowExchange.h"
#include "Move_NNIClock.h"
#include "Move_NNINonclock.h"
#include "Move_NodeTimeSlideUniform.h"
#include "Move_RateAgeBetaShift.h"
#include "Move_RootTimeSlide.h"
#include "Move_SubtreeScale.h"
#include "Move_TreeScale.h"
#include "Move_WeightedNodeTimeSlide.h"

/* Math types (in folder "datatypes/math") */
#include "RealMatrix.h"
#include "RealSymmetricMatrix.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"

/// Distributions ///

/* Distribution types (in folder "distributions") */

/* Character evolution models (in folder "distributions/evolution/character") */
#include "Dist_phyloCTMC.h"

/* Branch rate priors (in folder "distributions/evolution/tree") */
#include "Dist_branchRateJumpProcess.h"
#include "Dist_brownian.h"
#include "Dist_whiteNoise.h"

/* Tree priors (in folder "distributions/evolution/tree") */
#include "Dist_bdp.h"
#include "Dist_constPopMultispCoal.h"
#include "Dist_divDepYuleProcess.h"
#include "Dist_serialBDP.h"
#include "Dist_skySerialBDP.h"
#include "Dist_skyFossilBDP.h"
#include "Dist_uniformTimeTree.h"
#include "Dist_uniformTopology.h"

/* Distributions on simple variables (in folder "distributions/math") */
#include "Dist_bernoulli.h"
#include "Dist_beta.h"
#include "Dist_bimodalLnorm.h"
#include "Dist_bimodalNorm.h"
#include "Dist_dirichlet.h"
#include "Dist_exponential.h"
#include "Dist_gamma.h"
#include "Dist_geom.h"
#include "Dist_poisson.h"
#include "Dist_lnorm.h"
#include "Dist_norm.h"
#include "Dist_offsetExponential.h"
#include "Dist_offsetLnorm.h"
#include "Dist_oneOverX.h"
#include "Dist_positiveUnif.h"
#include "Dist_unif.h"
#include "Dist_wishart.h"

/* Mixture distributions (in folder "distributions/mixture") */
#include "Dist_dpp.h"

/// Functions ///

/* Helper functions for creating functions (in folder "functions") */
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantile.h"
#include "DistributionFunctionRv.h"


/* Argument rules (in folder "functions/argumentrules") */
#include "ArgumentRule.h"


/* Basic functions (in folder "functions/basic"). */

/* These are core functions for the Rev environment, providing user help
   and other essential services. */

#include "Func_citation.h"
#include "Func_clear.h"
#include "Func_contributors.h"
#include "Func_help.h"
#include "Func_license.h"
#include "Func_ls.h"
#include "Func_quit.h"
#include "Func_range.h"
#include "Func_rlvector.h"
#include "Func_seed.h"
#include "Func_seq.h"
#include "Func_structure.h"
#include "Func_type.h"
#include "Func_vector.h"


/* Functions related to evolution (in folder "functions/evolution") */
#include "Func_clade.h"
#include "Func_expBranchTree.h"
#include "Func_phyloRateMultiplier.h"
#include "Func_tmrca.h"
#include "Func_treeHeight.h"
#include "Func_treeAssembly.h"

/* Rate matrix functions (in folder "functions/evolution/ratematrix") */
#include "Func_blosum62.h"
#include "Func_cpRev.h"
#include "Func_dayhoff.h"
#include "Func_f81.h"
#include "Func_gtr.h"
#include "Func_hky.h"
#include "Func_jc.h"
#include "Func_jones.h"
#include "Func_mtRev.h"
#include "Func_mtMam.h"
#include "Func_rtRev.h"
#include "Func_vt.h"
#include "Func_wag.h"


/* Inference functions (in folder "functions/inference") */

/* Internal functions (in folder ("functions/internal") */

/* These are functions that are typically not called explicitly but implicitly
   through parsing of a Rev statement. Examples include a statement like '1 + 2',
   which results in the builtin '_add' function being called.
 
   Exceptions include Func_range and Func_vector, which are both used for implicit
   and explicit calls. They are therefore considered basic functions instead of
   internal functions.
 
   All internal functions have function calls that start with an underscore character,
   and therefore their class names have two underscore characters. They are typically
   templated. */

#include "Func__and.h"
#include "Func__eq.h"
#include "Func__ge.h"
#include "Func__gt.h"
#include "Func__le.h"
#include "Func__lt.h"
#include "Func__ne.h"
#include "Func__or.h"
#include "Func__unot.h"
#include "Func__add.h"
#include "Func__div.h"
#include "Func__mult.h"
#include "Func__mod.h"
#include "Func__sub.h"
#include "Func__uminus.h"


/* Input/output functions (in folder "functions/io") */
#include "Func_mapTree.h"
#include "Func_readCharacterData.h"
#include "Func_readTrace.h"
#include "Func_readTrees.h"
#include "Func_readTreeTrace.h"
#include "Func_source.h"
#include "Func_write.h"
#include "Func_writeFasta.h"
#include "Func_writeNexus.h"


/* Math functions (in folder "functions/math") */
#include "Func_abs.h"
#include "Func_ceil.h"
#include "Func_exp.h"
#include "Func_floor.h"
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_mean.h"
#include "Func_normalize.h"
#include "Func_power.h"
#include "Func_powermix.h"
#include "Func_round.h"
#include "Func_simplex.h"
#include "Func_sqrt.h"
#include "Func_trunc.h"


/* Statistics functions (in folder "functions/statistics") */
/* These are functions related to statistical distributions */
#include "Func_dppConcFromMean.h"
#include "Func_dppMeanFromConc.h"
#include "Func_dppNumTablesStatistic.h"


/** Initialize global workspace */
void RevLanguage::Workspace::initializeGlobalWorkspace(void)
{
    
    try
    {
        /* Add types: add a dummy variable which we use for type checking, conversion checking and other tasks. */
        
        /* Add base types (in folder "datatypes") */


        /* Add primitive types (in folder "datatypes/basic") (alphabetic order) */
        addType( new Complex()                  );
        addType( new Integer()                  );
        addType( new Natural()                  );
        addType( new Probability()              );
        addType( new Real()                     );
        addType( new RealPos()                  );
        addType( new RlBoolean()                );
        addType( new RlString()                 );
        
        /* Add container types (in folder "datatypes/container") (alphabetic order) */
        addType( new Vector<Integer>()          );
        addType( new Vector<Natural>()          );
        addType( new Vector<Real>()             );
        addType( new Vector<RealPos>()          );
        addType( new Vector<RlBoolean>()        );
        addType( new Vector<RlString>()         );
        

        /* Add evolution types (in folder "datatypes/evolution") (alphabetic order) */

        
        /* Add character types (in folder "datatypes/evolution/character") (alphabetic order) */

        
        /* Add tree types (in folder "datatypes/evolution/trees") (alphabetic order) */
        addTypeWithConstructor( "clade",            new Clade() );


        /* Add inference types (in folder "datatypes/inference") (alphabetic order) */
        addTypeWithConstructor( "beca",                   new BurninEstimationConvergenceAssessment()   );
        addTypeWithConstructor( "mcmc",                   new Mcmc()                                    );
        addTypeWithConstructor( "model",                  new Model()                                   );
        addTypeWithConstructor( "pmcmcmc",                new ParallelMcmcmc()                          );
        addTypeWithConstructor( "pathSampler",            new PathSampler()                             );
        addTypeWithConstructor( "powerPosterior",         new PowerPosterior()                          );
        addTypeWithConstructor( "steppingStoneSampler",   new SteppingStoneSampler()                    );


        ////////////////////////////////////////////////////////////////////////////////
        /* Add monitors (in folder "datatypes/inference/monitors") (alphabetic order) */
        ////////////////////////////////////////////////////////////////////////////////

        addTypeWithConstructor("mnExtNewick", new Mntr_ExtendedNewickFile());
        addTypeWithConstructor("mnFile",      new Mntr_File());
        addTypeWithConstructor("mnModel",     new Mntr_Model());
        addTypeWithConstructor("mnScreen",    new Mntr_Screen());

        // Nonstandard constructor names (for backward compatibility)
        addTypeWithConstructor("extNewickmonitor", new Mntr_ExtendedNewickFile());
        addTypeWithConstructor("filemonitor",      new Mntr_File());
        addTypeWithConstructor("modelmonitor",     new Mntr_Model());
        addTypeWithConstructor("screenmonitor",    new Mntr_Screen());
        
        
        ///////////////////////////////////////////////////////////////////////////////////
        /* Add moves (in folder "datatypes/inference/moves") (grouped by parameter type) */
        ///////////////////////////////////////////////////////////////////////////////////
        
        /* Regular moves (in folder "datatypes/inference/moves") (grouped by parameter type) */
        
        /* Moves on real values */
        addTypeWithConstructor("mvScale",               new Move_Scale() );
        addTypeWithConstructor("mvSlide",               new Move_Slide() );
        
        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mScale",                new Move_Scale() );
        addTypeWithConstructor("mSlide",                new Move_Slide() );
        

        /* Moves on integer values */
        addTypeWithConstructor("mvRandomIntegerWalk",   new Move_RandomIntegerWalk() );
        addTypeWithConstructor("mvRandomGeometricWalk", new Move_RandomGeometricWalk() );

        addTypeWithConstructor("mRandomIntegerWalk",    new Move_RandomIntegerWalk() );
        addTypeWithConstructor("mRandomGeometricWalk",  new Move_RandomGeometricWalk() );


        /* Moves on simplices */
        addTypeWithConstructor("mvSimplex",             new Move_Simplex() );
        addTypeWithConstructor("mvSimplexElementScale", new Move_SimplexSingleElementScale() );

        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mSimplex",              new Move_Simplex() );
        addTypeWithConstructor("mSimplexElementScale",  new Move_SimplexSingleElementScale() );
        

        /* Moves on vectors of real values */
        addTypeWithConstructor("mvRlcRateScale",                new Move_RLCRateScale() );
        addTypeWithConstructor("mvSingleElementScale",          new Move_SingleElementScale() );
        addTypeWithConstructor("mvSwitchRateJump",              new Move_SwitchRateJump() );
        addTypeWithConstructor("mvVectorScale",                 new Move_VectorScale() );
        addTypeWithConstructor("mvVectorSingleElementScale",    new Move_VectorSingleElementScale() );
        addTypeWithConstructor("mvVectorSingleElementSliding",  new Move_VectorSingleElementSlide() );

        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mRlcRateScale",                 new Move_RLCRateScale() );
        addTypeWithConstructor("mSingleElementScale",           new Move_SingleElementScale() );
        addTypeWithConstructor("mSwitchRateJump",               new Move_SwitchRateJump() );
        addTypeWithConstructor("mVectorScale",                  new Move_VectorScale() );
        addTypeWithConstructor("mVectorSingleElementScale",     new Move_VectorSingleElementScale() );
        addTypeWithConstructor("mVectorSingleElementSliding",   new Move_VectorSingleElementSlide() );

        
        /* Tree proposals (in folder "datatypes/inference/moves/tree") */
        addTypeWithConstructor("mvFNPR",                    new Move_FNPR() );
        addTypeWithConstructor("mvNarrow",                  new Move_NarrowExchange() );
        addTypeWithConstructor("mvNNI",                     new Move_NNIClock() );
        addTypeWithConstructor("mvNNI",                     new Move_NNINonclock() );
        addTypeWithConstructor("mvNNIClock",                new Move_NNIClock() );
        addTypeWithConstructor("mvNNINonclock",             new Move_NNINonclock() );
        addTypeWithConstructor("mvNodeTimeSlideUniform",    new Move_NodeTimeSlideUniform() );
        addTypeWithConstructor("mvRateAgeBetaShift",        new Move_RateAgeBetaShift() );
        addTypeWithConstructor("mvRootTimeSlide",           new Move_RootTimeSlide() );
        addTypeWithConstructor("mvSubtreeScale",            new Move_SubtreeScale() );
        addTypeWithConstructor("mvTreeScale",               new Move_TreeScale() );

        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mFNPR",                 new Move_FNPR() );
        addTypeWithConstructor("mNarrow",               new Move_NarrowExchange() );
        addTypeWithConstructor("mNNI",                  new Move_NNIClock() );
        addTypeWithConstructor("mNNI",                  new Move_NNINonclock() );
        addTypeWithConstructor("mNodeTimeSlideUniform", new Move_NodeTimeSlideUniform() );
        addTypeWithConstructor("mRateAgeBetaShift",     new Move_RateAgeBetaShift() );
        addTypeWithConstructor("mRootTimeSlide",        new Move_RootTimeSlide() );
        addTypeWithConstructor("mSubtreeScale",         new Move_SubtreeScale() );
        addTypeWithConstructor("mTreeScale",            new Move_TreeScale() );
        
        
        /* Add math types (in folder "datatypes/math") */
        addType( new RateMatrix()           );
        addType( new RealMatrix()           );
        addType( new Simplex()              );
        

        ///////////////////////////////////////////////////
        /* Add distributions (in folder "distributions") */
        ///////////////////////////////////////////////////
        
        /* Evolutionary processes (in folder "distributions/evolution") */

        /* Branch rate processes (in folder "distributions/evolution/branchrate") */
        
        // branch-rate jump process
        addDistribution( "dnDist_branchRateJumpProcess", new Dist_branchRateJumpProcess() );
        addDistribution( "branchRateJumpProcess",   new Dist_branchRateJumpProcess() );
        
        // brownian motion
        addDistribution( "dnBrownian",  new Dist_brownian() );
        addDistribution( "brownian",    new Dist_brownian() );
        
        // white noise process
        addDistribution( "dnWhiteNoise",    new Dist_whiteNoise() );
        addDistribution( "whiteNoise",      new Dist_whiteNoise() );
        addDistribution( "whitenoise",      new Dist_whiteNoise() );
        
        
        /* Character state evolution processes (in folder "distributions/evolution/character") */
        
        // simple phylogenetic CTMC on fixed number of discrete states
        addDistribution( "dnPhyloCTMC", new Dist_phyloCTMC<TimeTree>() );
        addDistribution( "dnPhyloCTMC", new Dist_phyloCTMC<BranchLengthTree>() );
        addDistribution( "phyloCTMC",   new Dist_phyloCTMC<TimeTree>() );
        addDistribution( "phyloCTMC",   new Dist_phyloCTMC<BranchLengthTree>() );
        addDistribution( "substModel",  new Dist_phyloCTMC<TimeTree>() );
        addDistribution( "substModel",  new Dist_phyloCTMC<BranchLengthTree>() );

        
        /* Tree distributions (in folder "distributions/evolution/tree") */
        
        // constant rate birth-death process
        addDistribution( "dnBDP"                        , new Dist_bdp() );
        addDistribution( "bdp"                          , new Dist_bdp() );
        addDistribution( "cBDP"                         , new Dist_bdp() );
        addDistribution( "BirthDeathConstant"           , new Dist_bdp() );
        addDistribution( "BDConst"                      , new Dist_bdp() );
        
        // constant rate birth-death process with serially sampled tips
        addDistribution( "dnSerialBDP"                  , new Dist_serialBDP() );
        addDistribution( "serialBDP"                    , new Dist_serialBDP() );
        addDistribution( "BirthDeathConstantSerial"     , new Dist_serialBDP() );
        addDistribution( "BDConstSS"                    , new Dist_serialBDP() );

        // piecewise constant rate birth-death process with serially sampled tips
        addDistribution( "dnSkySerialBDP"               , new Dist_skySerialBDP() );
        addDistribution( "skySerialBDP"                 , new Dist_skySerialBDP() );
        addDistribution( "BirthDeathSkySerial"          , new Dist_skySerialBDP() );
        addDistribution( "BDSkySS"                      , new Dist_skySerialBDP() );

        // piecewise constant rate fossilized birth-death process with serially sampled fossils
        addDistribution( "dnSkyFossilBDP"               , new Dist_skyFossilBDP() );
        addDistribution( "skyFossilBDP"                 , new Dist_skyFossilBDP() );
        addDistribution( "FossilizedBirthDeath"         , new Dist_skyFossilBDP() );
        addDistribution( "FBD"                          , new Dist_skyFossilBDP() );

        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
        addDistribution( "dnDivDepYuleProcess"          , new Dist_divDepYuleProcess() );
        addDistribution( "dnDivDepPBP"                  , new Dist_divDepYuleProcess() );
        addDistribution( "divDepPBP"                    , new Dist_divDepYuleProcess() );
        addDistribution( "divDepYuleProcess"            , new Dist_divDepYuleProcess() );
        addDistribution( "diversityDependentPureBirthProcess", new Dist_divDepYuleProcess() );
        addDistribution( "PureBirthDiversityDependent"  , new Dist_divDepYuleProcess() );
        addDistribution( "PBDD"                         , new Dist_divDepYuleProcess() );
        
        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
        addDistribution( "dnConstPopMultispCoal"        , new Dist_constPopMultispCoal() );
        addDistribution( "MultispCoal"                  , new Dist_constPopMultispCoal() );
        addDistribution( "MultispeciesCoalescentConstantPopulationProcess", new Dist_constPopMultispCoal() );

        // uniform time tree distribution
        addDistribution( "dnUniformTimeTree"            , new Dist_uniformTimeTree() );
        addDistribution( "uniformTimeTree"              , new Dist_uniformTimeTree() );
        
        // uniform topology distribution
        addDistribution( "dnUniformTopology"            , new Dist_uniformTopology() );
        addDistribution( "uniformTopology"              , new Dist_uniformTopology() );
        
        
        /* Statistical distributions on simple variables (in folder "distributions/math") */
        
        // bernoulli distribution
        addDistribution( "dnBernoulli",     new Dist_bernoulli() );
        addDistribution( "bernoulli",       new Dist_bernoulli() );
        
        // beta distribution
        addDistribution( "dnBeta",          new Dist_beta() );
        addDistribution( "beta",            new Dist_beta() );
        
        // bimodal normal distribution
        addDistribution( "dnBimodalnorm",   new Dist_bimodalNorm() );
        addDistribution( "bimodalNorm",     new Dist_bimodalNorm() );
        addDistribution( "bnorm",           new Dist_bimodalNorm() );
        
        // bimodal lognormal distribution
        addDistribution( "dnBimodalLnorm",  new Dist_bimodalLnorm() );
        addDistribution( "bimodalLnorm",    new Dist_bimodalLnorm() );
        addDistribution( "blnorm",          new Dist_bimodalLnorm() );
        
        // dirichlet distribution
        addDistribution( "dnDirichlet",     new Dist_dirichlet() );
        addDistribution( "dirichlet",       new Dist_dirichlet() );
		
        // gamma distribution
        addDistribution( "dnGamma",         new Dist_gamma() );
        addDistribution( "gamma",           new Dist_gamma() );
        
        // geometric distribution
        addDistribution( "dnGeom",          new Dist_geom() );
        addDistribution( "geom",            new Dist_geom() );
        
        // poisson distribution
        addDistribution( "dnPoisson",       new Dist_poisson() );
        addDistribution( "poisson",         new Dist_poisson() );
        
        // exponential distribution
        addDistribution( "dnExponential",   new Dist_exponential() );
        addDistribution( "dnExponential",   new Dist_offsetExponential() );
        addDistribution( "exponential",     new Dist_exponential() );
        addDistribution( "exponential",     new Dist_offsetExponential() );
        
        // lognormal distribution
        addDistribution( "dnLnorm",         new Dist_lnorm() );
        addDistribution( "dnLnorm",         new Dist_offsetLnorm() );
        addDistribution( "lnorm",           new Dist_lnorm() );
        addDistribution( "lnorm",           new Dist_offsetLnorm() );
        
        // normal distribution
        addDistribution( "dnNorm",          new Dist_norm() );
        addDistribution( "norm",            new Dist_norm() );
        
        // 1/x distribution
        addDistribution( "dnOneOverX",      new Dist_oneOverX() );
        addDistribution( "oneOverX",        new Dist_oneOverX() );
        
        // uniform distribution
        addDistribution( "dnUnif",          new Dist_unif() );
        addDistribution( "dnUnif",          new Dist_positiveUnif() );
        addDistribution( "unif",            new Dist_unif() );
        addDistribution( "unif",            new Dist_positiveUnif() );
        
        // wishart distribution
        addDistribution( "dnWishart",       new Dist_wishart() );
        addDistribution( "wishart",         new Dist_wishart() );
        
        
        /* Mixture distributions (in folder "distributions/mixture") */
        
        // dirichlet process prior distribution
        addDistribution( "dnDPP",		    new Dist_dpp<Real>() );
		addDistribution( "dnDPP",			new Dist_dpp<RealPos>() );
		addDistribution( "dnDPP",			new Dist_dpp<Natural>() );
		addDistribution( "dnDPP",			new Dist_dpp<Integer>() );
		addDistribution( "dnDPP",			new Dist_dpp<Probability>() );
        addDistribution( "dpp",				new Dist_dpp<Real>() );
		addDistribution( "dpp",				new Dist_dpp<RealPos>() );
		addDistribution( "dpp",				new Dist_dpp<Natural>() );
		addDistribution( "dpp",				new Dist_dpp<Integer>() );
		addDistribution( "dpp",				new Dist_dpp<Probability>() );
        
        // TAH: these don't seem to work with the moves, probably need to figure this out
		//addDistribution( "dpp",				new Dist_dpp<Topology>() );
		//addDistribution( "dpp",				new Dist_dpp<Simplex>() );
		//addDistribution( "dpp",				new Dist_dpp< Vector<RealPos> >() );
        

        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;
        

        ///////////////////////////////////////////
        /* Add functions (in "functions" folder) */
        ///////////////////////////////////////////
        
        /* Basic functions (in folder "functions/basic") */
        
        // regular functions
        addFunction( "citation",                 new Func_citation()                 );
        addFunction( "clear",                    new Func_clear()                    );
        addFunction( "contributors",             new Func_contributors()             );
        addFunction( "help",                     new Func_help()                     );
        addFunction( "license",                  new Func_license()                  );
        addFunction( "ls",                       new Func_ls()                       );
        addFunction( "q",                        new Func_quit()                     );
        addFunction( "quit",                     new Func_quit()                     );
        addFunction( "range",                    new Func_range()                    );
        addFunction( "seed",                     new Func_seed()                     );
        addFunction( "seq",                      new Func_seq<Integer>()             );
        addFunction( "seq",                      new Func_seq<Real>()                );
        addFunction( "str",                      new Func_structure()                );
        addFunction( "structure",                new Func_structure()                );
        addFunction( "type",                     new Func_type()                     );

        // vector functions
        addFunction( "v",         new Func_rlvector<Monitor>()                  );
        addFunction( "v",         new Func_rlvector<Move>()                     );
        addFunction( "v",         new Func_rlvector<AbstractCharacterData>()    );
        addFunction( "v",         new Func_vector<Natural>()                    );
        addFunction( "v",         new Func_vector<Integer>()                    );
        addFunction( "v",         new Func_vector<Real>()                       );
        addFunction( "v",         new Func_vector<RealPos>()                    );
        addFunction( "v",         new Func_vector<RlBoolean>()                  );
        addFunction( "v",         new Func_vector<Clade>()                      );
        addFunction( "v",         new Func_vector<RlString>()                   );
        addFunction( "v",         new Func_vector<TimeTree>()                   );
        
        
        /* Evolution-related functions (in folder "functions/evolution") */
        addFunction( "clade",                       new Func_clade()                    );
        addFunction( "expBranchTree",               new Func_expBranchTree()            );
        addFunction( "phyloRateMultiplier",         new Func_phyloRateMultiplier()      );
        addFunction( "tmrca",                       new Func_tmrca()                    );
        addFunction( "treeAssembly",                new Func_treeAssembly()             );
        addFunction( "treeHeight",                  new Func_treeHeight()               );
        
        // nonstandard names (for backward compatibility)
        addFunction( "expbranchtree",               new Func_expBranchTree()            );
        addFunction( "rateMultiplierPhyloFunction", new Func_phyloRateMultiplier()      );

        /* Rate matrix generator functions (in folder "functions/evolution/ratematrix") */
        addFunction( "blosum62", new Func_blosum62());
        addFunction( "cpRev",    new Func_cpRev()   );
        addFunction( "dayhoff",  new Func_dayhoff() );
        addFunction( "f81",      new Func_f81()     );
        addFunction( "gtr",      new Func_gtr()     );
        addFunction( "hky",      new Func_hky()     );
        addFunction( "jc",       new Func_jc()      );
        addFunction( "jones",    new Func_jones()   );
        addFunction( "mtMam",    new Func_mtMam()   );
        addFunction( "mtRev",    new Func_mtRev()   );
        addFunction( "rtRev",    new Func_rtRev()   );
        addFunction( "vt",       new Func_vt()      );
        addFunction( "wag",      new Func_wag()     );
        
        // nonstandard names (for backwards compatibility)
        addFunction( "Blosum62", new Func_blosum62());
        addFunction( "CpRev",    new Func_cpRev()   );
        addFunction( "Dayhoff",  new Func_dayhoff() );
        addFunction( "F81",      new Func_f81()     );
        addFunction( "HKY",      new Func_hky()     );
        addFunction( "JC",       new Func_jc()      );
        addFunction( "Jones",    new Func_jones()   );
        addFunction( "MtMam",    new Func_mtMam()   );
        addFunction( "MtRev",    new Func_mtRev()   );
        addFunction( "RtRev",    new Func_rtRev()   );
        addFunction( "VT",       new Func_vt()      );
        addFunction( "WAG",      new Func_wag()     );


        /* Inference functions (in folder "functions/inference") */

        
        /* Internal functions (in folder "functions/internal") */
        
        /* Note: These are functions that are called implicitly, and the name of which, if
         called explicitly, starts with an underscore character. */
        
        // not templated logical functions
        addFunction( "_and",      new Func__and()   );
        addFunction( "_or",       new Func__or()    );
        addFunction( "_unot",     new Func__unot()  );
        
        // range function (x:y)
        addFunction( "_range",    new Func_range()  );
        
        // logical templated functions
        addFunction( "_eq",       new Func__eq<             Integer,        Integer >()             );
        addFunction( "_eq",       new Func__eq<                Real,           Real >()             );
        addFunction( "_eq",       new Func__eq<             Integer,           Real >()             );
        addFunction( "_eq",       new Func__eq<                Real,        Integer >()             );
        addFunction( "_eq",       new Func__eq<           RlBoolean,      RlBoolean >()             );
        addFunction( "_eq",       new Func__eq<           RlString,       RlString  >()             );
        addFunction( "_ge",       new Func__ge<             Integer,        Integer >()             );
        addFunction( "_ge",       new Func__ge<                Real,           Real >()             );
        addFunction( "_ge",       new Func__ge<             Integer,           Real >()             );
        addFunction( "_ge",       new Func__ge<                Real,        Integer >()             );
        addFunction( "_ge",       new Func__ge<           RlBoolean,      RlBoolean >()             );
        addFunction( "_gt",       new Func__gt<             Integer,        Integer >()             );
        addFunction( "_gt",       new Func__gt<                Real,           Real >()             );
        addFunction( "_gt",       new Func__gt<           RlBoolean,      RlBoolean >()             );
        addFunction( "_le",       new Func__le<             Integer,        Integer >()             );
        addFunction( "_le",       new Func__le<                Real,           Real >()             );
        addFunction( "_le",       new Func__le<             Integer,           Real >()             );
        addFunction( "_le",       new Func__le<                Real,        Integer >()             );
        addFunction( "_le",       new Func__le<           RlBoolean,      RlBoolean >()             );
        addFunction( "_lt",       new Func__lt<             Integer,        Integer >()             );
        addFunction( "_lt",       new Func__lt<                Real,           Real >()             );
        addFunction( "_lt",       new Func__lt<           RlBoolean,      RlBoolean >()             );
        addFunction( "_ne",       new Func__ne<             Integer,        Integer >()             );
        addFunction( "_ne",       new Func__ne<                Real,           Real >()             );
        addFunction( "_ne",       new Func__ne<             Integer,           Real >()             );
        addFunction( "_ne",       new Func__ne<                Real,        Integer >()             );
        addFunction( "_ne",       new Func__ne<           RlBoolean,      RlBoolean >()             );
        
        // unary minus (e.g. -a)
        addFunction( "_uminus",   new Func__uminus<Integer, Integer>()  );
        addFunction( "_uminus",   new Func__uminus<Natural, Integer>()  );
        addFunction( "_uminus",   new Func__uminus<Real, Real>()        );
        addFunction( "_uminus",   new Func__uminus<RealPos, Real>()     );
        
        // addition (e.g. a+b )
        addFunction( "_add",      new Func__add< Natural         , Natural           , Natural           >(  )  );
        addFunction( "_add",      new Func__add< Integer         , Integer           , Integer           >(  )  );
        addFunction( "_add",      new Func__add< Real            , Real              , Real              >(  )  );
        addFunction( "_add",      new Func__add< RealPos         , RealPos           , RealPos           >(  )  );
        addFunction( "_add",      new Func__add< RlString        , RlString          , RlString          >(  )  );
        addFunction( "_add",      new Func__add< RlString        , Real              , RlString          >(  )  );
        addFunction( "_add",      new Func__add< RlString        , Integer           , RlString          >(  )  );
        addFunction( "_add",      new Func__add< Vector<Natural> , Vector<Natural>   , Vector<Natural>   >(  )  );
        addFunction( "_add",      new Func__add< Vector<Integer> , Vector<Integer>   , Vector<Integer>   >(  )  );
        addFunction( "_add",      new Func__add< Vector<RealPos> , Vector<RealPos>   , Vector<RealPos>   >(  )  );
        addFunction( "_add",      new Func__add< Vector<Real>    , Vector<Real>      , Vector<Real>      >(  )  );
        
        // division
        addFunction( "_div",      new Func__div< Natural         , Natural           , RealPos           >(  )  );
        addFunction( "_div",      new Func__div< Integer         , Integer           , Real              >(  )  );
        addFunction( "_div",      new Func__div< Real            , Real              , Real              >(  )  );
        addFunction( "_div",      new Func__div< RealPos         , RealPos           , RealPos           >(  )  );
        addFunction( "_div",      new Func__div< Vector<Natural> , Vector<Natural>   , Vector<RealPos>   >(  )  );
        addFunction( "_div",      new Func__div< Vector<Integer> , Vector<Integer>   , Vector<Real>      >(  )  );
        addFunction( "_div",      new Func__div< Vector<RealPos> , Vector<RealPos>   , Vector<RealPos>   >(  )  );
        addFunction( "_div",      new Func__div< Vector<Real>    , Vector<Real>      , Vector<RealPos>   >(  )  );
        
        // multiplication
        addFunction( "_mul",      new Func__mult< Natural            , Natural           , Natural           >(  )  );
        addFunction( "_mul",      new Func__mult< Integer            , Integer           , Integer           >(  )  );
        addFunction( "_mul",      new Func__mult< Real               , Real              , Real              >(  )  );
        addFunction( "_mul",      new Func__mult< RealPos            , RealPos           , RealPos           >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<Natural>    , Vector<Natural>   , Vector<Natural>   >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<Integer>    , Vector<Integer>   , Vector<Integer>   >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<RealPos>    , Vector<RealPos>   , Vector<RealPos>   >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<Real>       , Vector<Real>      , Vector<Real>      >(  )  );
        
        // subtraction
        addFunction( "_sub",      new Func__sub< Integer         , Integer           , Integer           >(  )  );
        addFunction( "_sub",      new Func__sub< Real            , Real              , Real              >(  )  );
        addFunction( "_sub",      new Func__sub< Vector<Integer> , Vector<Integer>   , Vector<Integer>   >(  )  );
        addFunction( "_sub",      new Func__sub< Vector<Real>    , Vector<Real>      , Vector<Real>      >(  )  );
        
        // modulo
        addFunction( "_mod",      new Func__mod() );
        
        // exponentiation
        addFunction( "_exp",      new Func_power() );
        

        /* Input/output functions (in folder "functions/io") */
        addFunction( "mapTree",                     new Func_mapTree<BranchLengthTree>()   );
        addFunction( "mapTree",                     new Func_mapTree<TimeTree>()           );
        addFunction( "readCharacterData",           new Func_readCharacterData()           );
        addFunction( "readTrace",                   new Func_readTrace()                   );
        addFunction( "readTrees",                   new Func_readTrees()                   );
        addFunction( "readTreeTrace",               new Func_readTreeTrace()               );
        addFunction( "source",                      new Func_source()                      );
        addFunction( "write",                       new Func_write()                       );
        addFunction( "writeFasta",                  new Func_writeFasta()                  );
        addFunction( "writeNexus",                  new Func_writeNexus()                  );

        
        /* Math functions (in folder "functions/math") */
		
		// absolute function
        addFunction( "abs",         new Func_abs()  );
		
		// ceil function
        addFunction( "ceil",        new Func_ceil<Real,Integer>()  );
        addFunction( "ceil",        new Func_ceil<RealPos,Natural>()  );
        addFunction( "ceiling",     new Func_ceil<Real,Integer>()  );
        addFunction( "ceiling",     new Func_ceil<RealPos,Natural>()  );
        
        // exponential function
        addFunction( "exp",         new Func_exp() );
		
		// floor function
        addFunction( "floor",       new Func_floor<Real,Integer>()  );
        addFunction( "floor",       new Func_floor<RealPos,Natural>()  );
        
        // natural log function
        addFunction( "ln",          new Func_ln()  );
        
        // log function
		addFunction( "log",         new Func_log()  );
        
        // mean function
		addFunction( "mean",        new Func_mean()  );
        
        // normalize vector function
		addFunction( "normalize", new Func_normalize()  );

		// power function
        addFunction( "power",     new Func_power() );

        // powermix function (TODO: remove when user functions work)
        addFunction( "powermix",  new Func_powermix() );
        
		// round function
        addFunction( "round",     new Func_round<Real,Integer>()  );
        addFunction( "round",     new Func_round<RealPos,Natural>()  );
		
        // simplex constructor function
        addFunction( "simplex",   new Func_simplex()                  );

		// square root function
        addFunction( "sqrt",      new Func_sqrt()  );
		
		// truncate function
        addFunction( "trunc",     new Func_trunc<Real,Integer>()  );
        addFunction( "trunc",     new Func_trunc<RealPos,Natural>()  );


 		/* Statistics functions (in folder "functions/statistics") */
        addFunction("dppConcFromMean",  new Func_dppConcFromMean( )     );
        addFunction("dppMeanFromConc",  new Func_dppMeanFromConc( )  );

        // nonstandard forms form backward compatibility
//        addFunction("dppCPFromNum",     new Func_dppConcFromMean( )     );
//        addFunction("dppNumFromCP",     new Func_dppMeanFromConc( )  );

        
        ///////////////////////////////////////////////////////////////////////////
        /* Add distribution functions (using help classes in folder "functions") */
        ///////////////////////////////////////////////////////////////////////////
        
        // bernoulli distribution
        addFunction("dbernoulli", new DistributionFunctionPdf<Natural>( new Dist_bernoulli() ) );
        addFunction("rbernoulli", new DistributionFunctionRv<Natural>( new Dist_bernoulli() ) );
        
        // beta distribution
        addFunction("dbeta", new DistributionFunctionPdf<Probability>( new Dist_beta() ) );
        //        addFunction("pbeta", new DistributionFunctionCdf( new Dist_beta() ) );
        //        addFunction("qbeta", new DistributionFunctionQuantile( new Dist_beta() ) );
        addFunction("rbeta", new DistributionFunctionRv<Probability>( new Dist_beta() ) );
        
        // exponential distribution
        addFunction("dexponential", new DistributionFunctionPdf<RealPos>( new Dist_exponential() ) );
        addFunction("pexponential", new DistributionFunctionCdf( new Dist_exponential() ) );
        addFunction("qexponential", new DistributionFunctionQuantile( new Dist_exponential() ) );
        addFunction("rexponential", new DistributionFunctionRv<RealPos>( new Dist_exponential() ) );
        
        // gamma distribution
        addFunction("dgamma", new DistributionFunctionPdf<RealPos>( new Dist_gamma() ) );
        addFunction("pgamma", new DistributionFunctionCdf( new Dist_gamma() ) );
        addFunction("qgamma", new DistributionFunctionQuantile( new Dist_gamma() ) );
        addFunction("rgamma", new DistributionFunctionRv<RealPos>( new Dist_gamma() ) );
        
        // lognormal distribution
        addFunction("dlnorm", new DistributionFunctionPdf<RealPos>( new Dist_lnorm() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new Dist_lnorm() ) );
        addFunction("qlnorm", new DistributionFunctionQuantile( new Dist_lnorm() ) );
        addFunction("rlnorm", new DistributionFunctionRv<RealPos>( new Dist_lnorm() ) );
        addFunction("dlnorm", new DistributionFunctionPdf<Real>( new Dist_offsetLnorm() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new Dist_offsetLnorm() ) );
        addFunction("qlnorm", new DistributionFunctionQuantile( new Dist_offsetLnorm() ) );
        addFunction("rlnorm", new DistributionFunctionRv<Real>( new Dist_offsetLnorm() ) );
        
        // normal distribution
        addFunction("dnorm", new DistributionFunctionPdf<Real>( new Dist_norm() ) );
        addFunction("pnorm", new DistributionFunctionCdf( new Dist_norm() ) );
        addFunction("qnorm", new DistributionFunctionQuantile( new Dist_norm() ) );
        addFunction("rnorm", new DistributionFunctionRv<Real>( new Dist_norm() ) );
        
        // uniform distribution
        addFunction("dunif", new DistributionFunctionPdf<Real>( new Dist_unif() ) );
        addFunction("punif", new DistributionFunctionCdf( new Dist_unif() ) );
        addFunction("qunif", new DistributionFunctionQuantile( new Dist_unif() ) );
        addFunction("runif", new DistributionFunctionRv<Real>( new Dist_unif() ) );
        addFunction("dunif", new DistributionFunctionPdf<RealPos>( new Dist_positiveUnif() ) );
        addFunction("punif", new DistributionFunctionCdf( new Dist_positiveUnif() ) );
        addFunction("qunif", new DistributionFunctionQuantile( new Dist_positiveUnif() ) );
        addFunction("runif", new DistributionFunctionRv<RealPos>( new Dist_positiveUnif() ) );


    }
    catch(RbException& rbException) {
        
        RBOUT("Caught an exception while initializing the workspace\n");
        std::ostringstream msg;
        rbException.print(msg);
        msg << std::endl;
        RBOUT(msg.str());
        
        RBOUT("Please report this bug to the RevBayes Development Core Team");
        
        RBOUT("Press any character to exit the program.");
        getchar();
        exit(1);
    }
}


