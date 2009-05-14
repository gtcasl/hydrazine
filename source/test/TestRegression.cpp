#ifndef TEST_REGRESSION_CPP_INCLUDED
#define TEST_REGRESSION_CPP_INCLUDED

#include "TestRegression.h"

namespace test
{

	double TestRegression::percentError( const std::vector< double >& reference, const std::vector< double >& experimental )
	{
	
		double error;
		
		error = 0;
		
		assert( reference.size() == experimental.size() );
		
		std::vector< double >::const_iterator ri;
		std::vector< double >::const_iterator ei;		
		
		ei = experimental.begin();
		
		report( "Computing error" );
		
		for( ri = reference.begin(); ri != reference.end(); ++ri )
		{
		
			report( "Ref : " << *ri << ", Exp : " << *ei << ", error : " << ABS( ( ( *ri - *ei ) / *ei ) ) );
		
			error += ABS( ( ( *ri - *ei ) / (*ei + TOLERABLE_ERROR) ) );
		
			++ei;
		}
		
		return error/reference.size();
	
	}
	
	bool TestRegression::runSanity( std::string& stat )
	{
	
		std::stringstream stream;
	
		std::vector< double > samples;
		
		std::vector< double > dependentSamples;
		
		std::vector< double > computedSamples;
		
		std::vector< double > actualResult;
		
		std::vector< double > computedResult;
		
		samples.resize( 15 );
				
		dependentSamples.resize( 15 );
		computedSamples.resize( 15 );
		
		actualResult.resize( 5 );		
				
		samples[0] = 1.47;
		samples[1] = 1.50;
		samples[2] = 1.52;
		samples[3] = 1.55;
		samples[4] = 1.57;
		samples[5] = 1.60;
		samples[6] = 1.63;
		samples[7] = 1.65;
		samples[8] = 1.68;
		samples[9] = 1.70;
		samples[10] = 1.73;
		samples[11] = 1.75;
		samples[12] = 1.78;
		samples[13] = 1.81;
		samples[14] = 1.83;
		
		dependentSamples[0] = 52.21;
		dependentSamples[1] = 53.12;
		dependentSamples[2] = 54.48;
		dependentSamples[3] = 55.84;
		dependentSamples[4] = 57.2;
		dependentSamples[5] = 58.57;
		dependentSamples[6] = 59.93;
		dependentSamples[7] = 61.29;
		dependentSamples[8] = 63.11;
		dependentSamples[9] = 64.47;
		dependentSamples[10] = 66.28;
		dependentSamples[11] = 68.1;
		dependentSamples[12] = 69.92;
		dependentSamples[13] = 72.17;
		dependentSamples[14] = 74.46;
	
		
		actualResult[0] = 129;
		actualResult[1] = -143;
		actualResult[2] = 62;
		actualResult[3] = 0;
		actualResult[4] = 0;
	
		if( verbose )
		{
	
			stream << "Actual polynomial : \n";
		
			for( unsigned int i = 0; i < 3; i++ )
			{
		
					stream << actualResult[i] << "*x^" << i;
			
					stream << " + ";			
		
			}
		
			stream << actualResult[3] << "*log2(x) + ";
		
			stream << actualResult[4] << "*e^x";
	
			stream << "\n";
		
		}
		
		common::polynomialRegressionLE( computedResult, samples, dependentSamples, 5 );
	
		if( verbose )
		{
	
			stream << "Computed polynomial : \n";
		
			for( unsigned int i = 0; i < 3; i++ )
			{
			
				stream << computedResult[i] << "*x^" << i;
		
				stream << " + ";
		
			}
	
			stream << computedResult[3] << "*log2(x) + ";
		
			stream << computedResult[4] << "*e^x";
	
			stream << "\n";
		
			
		}
		
		for( unsigned int i = 0; i < 15; i++ )
		{
		
			computedSamples[i] = 0;
		
			for( unsigned int j = 0; j < 3; j++ )
			{
			
				computedSamples[i] += computedResult[j] * pow( samples[i], j );	
			
			}
			
			computedSamples[i] += computedResult[3] * log2( samples[i] );
			
			computedSamples[i] += computedResult[4] * exp( samples[i] );
		
		}
				
		double error;
		
		error = percentError( dependentSamples, computedSamples );
		
		if( verbose )
		{
		
			stream << "Error = " << error << "\n";
		
		}
		
		stat += stream.str();
		
		return error < TOLERABLE_ERROR;	
	}


	bool TestRegression::runRegressionLE( std::string& stat )
	{

		std::stringstream stream;

		unsigned int order;
		
		unsigned int samples;

		order = this->order + 2;

		samples = MAX( this->samples, order * 2 );
	
		std::vector< double > sampleVector;
		
		std::vector< double > dependentSamples;
		
		std::vector< double > computedSamples;
		
		std::vector< double > actualResults;
		
		std::vector< double > computedResults;
		
		sampleVector.resize( samples );
		
		dependentSamples.resize( samples );
		
		computedSamples.resize( samples );
		
		actualResults.resize( order );
		
		for( unsigned int i = 0; i < samples; i++ )
		{
		
			sampleVector[i] = ( rand() % MAX_SAMPLE ) + MIN_SAMPLE;
		
		}
		
		stream << "Actual polynomial : \n";
		
		for( unsigned int i = 0; i < order; i++ )
		{
		
			actualResults[i] = rand() % MAX_COEFFICIENT;
			
			if( i < order-2) 
			{
			
				stream << actualResults[i] << "*x^" << i;
			
				stream << " + ";
			
			}
		
		}
		
		stream << actualResults[order-2] << "*log2(x) + ";
		
		stream << actualResults[order-1] << "*e^x";
		
		stream << "\n";
		
		for( unsigned int i = 0; i < samples; i++ )
		{
		
			dependentSamples[i] = 0;
		
			for( unsigned int j = 0; j < order-2; j++ )
			{
		
				dependentSamples[i] += actualResults[j] * pow( sampleVector[i], j );
		
			}
			
			dependentSamples[i] += actualResults[order-2] * log2( sampleVector[i] );
		
			dependentSamples[i] += actualResults[order-1] * exp( sampleVector[i] );
		
		}
		
		common::polynomialRegressionLE( computedResults, sampleVector, dependentSamples, order );
	
		stream << "Computed polynomial : \n";
		
		for( unsigned int i = 0; i < order-2; i++ )
		{
		
			stream << computedResults[i] << "*x^" << i;
		
			stream << " + ";			
		
		}
	
		stream << computedResults[order-2] << "*log2(x) + ";
		
		stream << computedResults[order-1] << "*e^x";
	
		stream << "\n";
		
		for( unsigned int i = 0; i < sampleVector.size(); i++ )
		{
		
			computedSamples[i] = 0;
		
			for( unsigned int j = 0; j < order-2; j++ )
			{
			
				computedSamples[i] += computedResults[j] * pow( sampleVector[i], j );	
			
			}
			
			computedSamples[i] += computedResults[order-2] * log2( sampleVector[i] );
			
			computedSamples[i] += computedResults[order-1] * exp( sampleVector[i] );
		
		}
				
		double error;
		
		error = percentError( dependentSamples, computedSamples );
		
		if( verbose )
		{
		
			stream << "Error = " << error << "\n";
		
		}
		
		stat += stream.str();
		
		return error < TOLERABLE_ERROR;
	
	}

	bool TestRegression::runRegressionL( std::string& stat )
	{

		std::stringstream stream;
	
		unsigned int order;
		
		unsigned int samples;

		order = this->order + 1;

		samples = MAX( this->samples, order * 2 );
	
		std::vector< double > sampleVector;
		
		std::vector< double > dependentSamples;
		
		std::vector< double > computedSamples;
		
		std::vector< double > actualResults;
		
		std::vector< double > computedResults;
		
		sampleVector.resize( samples );
		
		dependentSamples.resize( samples );
		computedSamples.resize( samples );
		
		actualResults.resize( order );
		
		for( unsigned int i = 0; i < samples; i++ )
		{
		
			sampleVector[i] = ( rand() % MAX_SAMPLE ) + MIN_SAMPLE;
		
		}
		
		stream << "Actual polynomial : \n";
		
		for( unsigned int i = 0; i < order; i++ )
		{
		
			actualResults[i] = rand() % MAX_COEFFICIENT;
			
			if( i < order-1) 
			{
			
				stream << actualResults[i] << "*x^" << i;
			
				stream << " + ";
			
			}
		
		}
		
		stream << actualResults[order-1] << "*log2(x) + ";
				
		stream << "\n";
		
		for( unsigned int i = 0; i < samples; i++ )
		{
		
			dependentSamples[i] = 0;
		
			for( unsigned int j = 0; j < order-1; j++ )
			{
		
				dependentSamples[i] += actualResults[j] * pow( sampleVector[i], j );
		
			}
			
			dependentSamples[i] += actualResults[order-1] * log2( sampleVector[i] );
		
		}
		
		common::polynomialRegressionL( computedResults, sampleVector, dependentSamples, order );
	
		stream << "Computed polynomial : \n";
		
		for( unsigned int i = 0; i < order-1; i++ )
		{
		
			stream << computedResults[i] << "*x^" << i;
		
			stream << " + ";			
		
		}
	
		stream << computedResults[order-1] << "*log2(x) + ";
		
		stream << "\n";
		
		for( unsigned int i = 0; i < sampleVector.size(); i++ )
		{
		
			computedSamples[i] = 0;
		
			for( unsigned int j = 0; j < order-1; j++ )
			{
			
				computedSamples[i] += computedResults[j] * pow( sampleVector[i], j );	
			
			}
			
			computedSamples[i] += computedResults[order-1] * log2( sampleVector[i] );
			
		}
		
		double error;
		
		error = percentError( dependentSamples, computedSamples );
		
		if( verbose )
		{
		
			stream << "Error = " << error << "\n";
		
		}
		
		stat += stream.str();
		
		return error < TOLERABLE_ERROR;
	
	}

	bool TestRegression::runRegression( std::string& stat )
	{

		std::stringstream stream;

		unsigned int order;
		
		unsigned int samples;

		order = this->order;

		samples = MAX( this->samples, order * 2 );
	
		std::vector< double > sampleVector;
		
		std::vector< double > dependentSamples;
		
		std::vector< double > computedSamples;
		
		std::vector< double > actualResults;
		
		std::vector< double > computedResults;
		
		sampleVector.resize( samples );
		
		dependentSamples.resize( samples );
		computedSamples.resize( samples );
		
		actualResults.resize( order );
		
		for( unsigned int i = 0; i < samples; i++ )
		{
		
			sampleVector[i] = ( rand() % MAX_SAMPLE ) + MIN_SAMPLE;
		
		}
		
		stream << "Actual polynomial : \n";
		
		for( unsigned int i = 0; i < order; i++ )
		{
		
			actualResults[i] = rand() % MAX_COEFFICIENT;
			
			if( i < order-1) 
			{
			
				stream << actualResults[i] << "*x^" << i;
			
				stream << " + ";
			
			}
		
		}
		
		stream << actualResults[order - 1] << "*x^" << (order - 1);
				
		stream << "\n";
		
		for( unsigned int i = 0; i < samples; i++ )
		{
		
			dependentSamples[i] = 0;
		
			for( unsigned int j = 0; j < order; j++ )
			{
		
				dependentSamples[i] += actualResults[j] * pow( sampleVector[i], j );
		
			}
			
		}
		
		common::polynomialRegression( computedResults, sampleVector, dependentSamples, order );
	
		stream << "Computed polynomial : \n";
		
		for( unsigned int i = 0; i < order-1; i++ )
		{
		
			stream << computedResults[i] << "*x^" << i;
		
			stream << " + ";			
		
		}
	
		stream << computedResults[order-1] << "*x^" << (order - 1);
		
		stream << "\n";
		
		for( unsigned int i = 0; i < sampleVector.size(); i++ )
		{
		
			computedSamples[i] = 0;
		
			for( unsigned int j = 0; j < order; j++ )
			{
			
				computedSamples[i] += computedResults[j] * pow( sampleVector[i], j );	
			
			}
			
		}
		
		double error;
		
		error = percentError( dependentSamples, computedSamples );
		
		if( verbose )
		{
		
			stream << "Error = " << error << "\n";
		
		}
		
		stat += stream.str();
		
		return error < TOLERABLE_ERROR;
		
	}
	
	bool TestRegression::doTest( std::string& stat )
	{
	
		bool pass;
		
		pass = true;
		
		if( verbose )
		{
		
			stat += "\n\n---------------- SANITY TEST -----------------\n";
		
		}
		
		if( runSanity( stat ) )
		{
		
			stat += "Sanity test passed.\n";
		
		}
		else
		{
		
			stat += "Sanity test failed.\n";
			
			pass = false;
		
		}

		if( verbose )
		{
		
			stat += "\n\n---------------- POLYNOMIAL TEST -----------------\n";
		
		}
		
		if( runRegression( stat ) )
		{
		
			stat += "Polynomial regression test passed.\n";
		
		}
		else
		{
		
			stat += "Polynomial regression test failed.\n";
			
			pass = false;
		
		}

		if( verbose )
		{
		
			stat += "\n\n---------------- POLYNOMIAL + LOG TEST -----------------\n";
		
		}
		
		if( runRegressionL( stat ) )
		{
		
			stat += "Polynomial and Logarithmic regression test passed.\n";
		
		}
		else
		{
		
			stat += "Polynomial and Logarithmic regression test failed.\n";
		
			pass = false;
		
		}
		
		if( verbose )
		{
		
			stat += "\n\n---------------- POLYNOMIAL + LOG + EXPONENTIAL TEST -----------------\n";
		
		}
		
		if( runRegressionLE( stat ) )
		{
		
			stat += "Polynomial and Logarithmic and Exponential regression test passed.\n";
		
		}
		else
		{
		
			stat += "Polynomial and Logarithmic and Exponential regression test failed.\n";

			pass = false;
		
		}
		
		return pass;
	
	}

	TestRegression::TestRegression()
	{
	
		Name = "TestRegression";
		
		Description = "A test for the functionality of a polynomial regression.";
	
	}

}

int main( int argc, char* argv[] )
{

	common::ArgumentParser parser( argc, argv );
	
	test::TestRegression test;
	
	parser.description( test.description() );
	
	bool help;
	
	parser.parse( "-h", help, "Print this help message." );
		
	parser.parse( "-o", test.order, 5, "The order of the polynomial to use for the regression."  );
	parser.parse( "-s", test.samples, test.order*2, "The number of sample points to use for the regression."  );
	parser.parse( "-v", test.verbose, "Print out status information as the test is being run." );

	if( help )
	{
	
		std::cout << parser.help();
	
		return false;
	
	}

	test.test();

	if( test.verbose )
	{

		std::cout << test.toString();
	
	}

	return test.passed();

}

#endif

