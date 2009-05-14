#ifndef TEST_REGRESSION_H_INCLUDED
#define TEST_REGRESSION_H_INCLUDED

#include <common/implementation/polynomialRegression.h>
#include <common/implementation/ArgumentParser.h>
#include <common/interface/Test.h>

#include <iostream>
#include <iomanip>

#define MAX_SAMPLE 40
#define MAX_COEFFICIENT 10
#define SMALL_VALUE .01
#define MIN_SAMPLE SMALL_VALUE

#define TOLERABLE_ERROR .01

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#define REPORT_BASE 0

namespace test
{

	class TestRegression : public Test
	{
	
		private:
		
			double percentError( const std::vector< double >& reference, const std::vector< double >& experimental );

			bool runRegressionLE( std::string& status );

			bool runRegressionL( std::string& status );
			
			bool runRegression( std::string& status );

			bool runSanity( std::string& status );
		
			bool doTest( std::string& status );
			
		public:
		
			TestRegression();
			
			bool verbose;
			
			unsigned int order;
			
			unsigned int samples;
	
	};

}

int main( int argc, char* argv[] );

#endif

