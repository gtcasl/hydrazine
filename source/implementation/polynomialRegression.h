#ifndef POLYNOMIAL_REGRESSION_H_INCLUDED
#define POLYNOMIAL_REGRESSION_H_INCLUDED

#include <mtl/lu.h>
#include <mtl/blais.h>
#include <mtl/utils.h>
#include "debug.h"
#include "macros.h"

#ifdef REPORT_BASE
#undef REPORT_BASE
#endif

#ifndef LARGEST_EXP_SAMPLE
#define LARGEST_EXP_SAMPLE 400
#endif

#define REPORT_BASE 0

namespace common
{

	template< class T >
	T mean( std::vector< T >& array )
	{
	
		T total;
		
		total = 0;
		
		unsigned int size;
		
		size = array.size();
		
		for( unsigned int i = 0; i < size; i++ )
		{
		
			total += array[i] / size;
		
		}
		
		return total;
	
	}
	
	template< class T >
	T variance( std::vector< T >& array )
	{
	
		T Mean;
		
		Mean = mean( array );
		
		T meanSquared;
		
		meanSquared = pow( Mean, 2 );
		
		T Variance;
		
		Variance = 0;
		
		unsigned int size;
		
		size = array.size();
		
		for( unsigned int i = 0; i < size; i++ )
		{
		
			Variance += ( pow(array[i],2) - meanSquared  ) / size;
		
		}
		
		return Variance;
	
	}

	template< class T >
	void polynomialRegressionLE( std::vector< T >& coefficients, 
		const std::vector< T >& samples, 
		const std::vector< T >& dependentSamples, unsigned int order )
	{
	
		bool includeExp;
		
		includeExp = true;
	
		order = MAX(2, order);
	
		assert( order <= dependentSamples.size() );
	
		assert( dependentSamples.size() == samples.size() );
	
		coefficients.resize( order );
		
		typedef typename mtl::matrix< T, mtl::rectangle<>, mtl::dense<>, mtl::row_major >::type Matrix;
		
		typedef typename mtl::dense1D< T > Vector;
		
		Matrix array(samples.size(), order);
		
		Matrix transpose( order, samples.size() );
		
		Matrix left( order, order );
		
		Vector right( order );
		
		Vector y( samples.size() );
		
		Vector pvector( left.nrows(), 0 );
		
		for( unsigned int i = 0; i < samples.size(); i++ )
		{
		
			y[i] = dependentSamples[i];
		
		}
		
		for( unsigned int i = 0; i < array.nrows() ; i++ )
		{
		
			for( unsigned int j = 0; j < array.ncols() - 2; j++ )
			{
		
				array[i][j] = (T) pow( samples[i], j );
			
			}

			array[i][array.ncols() - 2] = (T) log2( samples[i] );
			
			if( samples[i] >= LARGEST_EXP_SAMPLE )
			{
			
				includeExp = false;
				
			}
			
		}
		
		if( includeExp )
		{
		
			for( unsigned int i = 0; i < array.nrows() ; i++ )
			{
		
				array[i][array.ncols() - 1] = (T) exp( samples[i] );
		
			}
		
		}
		else
		{
	
			for( unsigned int i = 0; i < array.nrows() ; i++ )
			{
		
				array[i][array.ncols() - 1] = 0;
		
			}
		
		}
		
		mtl::transpose( array, transpose );
		
		mtl::mult( transpose, array, left );
			
		mtl::mult( transpose, y, right );
		
		mtl::lu_factorize( left, pvector );
		
		if( mtl::min( pvector ) > 0 )
		{
		
			mtl::lu_solve( left, pvector, right, y );
		
			for( unsigned int i = 0; i < order; i++ )
			{
		
				coefficients[i] = y[i];
		
			}
	
		}
		else
		{
		
			for( unsigned int i = 0; i < order; i++ )
			{
		
				coefficients[i] = 0;
		
			}
		
		}
	
	}

	template< class T >
	void polynomialRegressionL( std::vector< T >& coefficients, const std::vector< T >& samples, const std::vector< T >& dependentSamples, unsigned int order )
	{
	
		order = MAX(1, order);
		assert( order <= dependentSamples.size() );
		assert( dependentSamples.size() == samples.size() );
	
		coefficients.resize( order );
		
		report( "Computing regressionL with " << coefficients.size() 
			<< " coefficients, " << samples.size() << " samples of order " 
			<< order );
		
		typedef typename mtl::matrix< T, mtl::rectangle<>, mtl::dense<>, mtl::row_major >::type Matrix;
		
		typedef typename mtl::dense1D< T > Vector;
		
		Matrix array( samples.size(), order);
		
		Matrix transpose( order, samples.size() );
		
		Matrix left( order, order );
		
		Vector right( order );
		
		Vector y( samples.size() );
		
		Vector pvector( left.nrows() );
		
		for( unsigned int i = 0; i < samples.size(); i++ )
		{
		
			y[i] = dependentSamples[i];
		
		}
		
		for( unsigned int i = 0; i < array.nrows() ; i++ )
		{
		
			for( unsigned int j = 0; j < array.ncols() - 1; j++ )
			{
		
				array[i][j] = (T) pow( samples[i], j );
			
			}

			array[i][array.ncols() - 1] = (T) log2( samples[i] );
				
		}
		
		mtl::transpose( array, transpose );
	
		mtl::mult( transpose, array, left );
		
		mtl::mult( transpose, y, right );
		
		mtl::lu_factorize( left, pvector );
		
		if( mtl::min( pvector ) > 0 )
		{
		
			mtl::lu_solve( left, pvector, right, y );
		
			for( unsigned int i = 0; i < order; i++ )
			{
		
				coefficients[i] = y[i];
		
			}
	
		}
		else
		{
		
			for( unsigned int i = 0; i < order; i++ )
			{
		
				coefficients[i] = 0;
		
			}
		
		}	

	}
	
	
	template< class T >
	void polynomialRegression( std::vector< T >& coefficients, const std::vector< T >& samples, const std::vector< T >& dependentSamples, unsigned int order )
	{
	
		assert( order <= dependentSamples.size() );
	
		assert( dependentSamples.size() == samples.size() );
	
		coefficients.resize( order );
		
		typedef typename mtl::matrix< T, mtl::rectangle<>, mtl::dense<>, mtl::row_major >::type Matrix;
		
		typedef typename mtl::dense1D< T > Vector;
		
		Matrix array(samples.size(), order);
		
		Matrix transpose( order, samples.size() );
		
		Matrix left( order, order );
		
		Vector right( order );
		
		Vector y( samples.size() );
		
		Vector pvector( left.nrows() );
		
		for( unsigned int i = 0; i < samples.size(); i++ )
		{
		
			y[i] = dependentSamples[i];
		
		}
		
		for( unsigned int i = 0; i < array.nrows() ; i++ )
		{
		
			for( unsigned int j = 0; j < array.ncols(); j++ )
			{
		
				array[i][j] = (T) pow( samples[i], j );
			
			}

			
		}
		
		mtl::transpose( array, transpose );
	
		mtl::mult( transpose, array, left );
		
		mtl::mult( transpose, y, right );

		mtl::lu_factorize( left, pvector );		
		
		if( mtl::min( pvector ) > 0 )
		{
		
			mtl::lu_solve( left, pvector, right, y );
		
			for( unsigned int i = 0; i < order; i++ )
			{
		
				coefficients[i] = y[i];
		
			}
	
		}
		else
		{
		
			for( unsigned int i = 0; i < order; i++ )
			{
		
				coefficients[i] = 0;
		
			}
		
		}	

	}
	
	template< class T >
	void residuals( std::vector< T >& result, std::vector< T >& samples, std::vector< T >& polynomial, T exponential, T logarithm )
	{
	
		unsigned int size;
	
		size = samples.size();
	
		result.resize( size );
	
		// compute the samples
		for( unsigned int i = 0; i < size; i++ )
		{
		
			T value;
			
			value = 0;
			
			unsigned int polySize;
			
			polySize = polynomial.size();
			
			for( unsigned int j = 0; j < polySize; j++ )
			{
			
				value += polynomial[j] * pow( samples[i], j );
		
			}
			
			value += exponential * log2( samples[i] );
			
			value += logarithm * exp( samples[i] );
			
			result[i] = samples[i] - value;
			
		}
	
	}

}


#endif
