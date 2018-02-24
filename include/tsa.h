// Copyright (c) 2018 Grumpy Cat Software S.L.
// 
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <arrayfire.h>
#include "version.h"
#include <vector>

using namespace af;


namespace tsa {
    
    const char* version();

    namespace simplification {

        // Type Point
        typedef std::pair<double, double> Point;

        void RamerDouglasPeucker(const std::vector<tsa::simplification::Point> pointList, double epsilon, std::vector<tsa::simplification::Point> &out);
        
        void visvalingam_simplify(std::vector<tsa::simplification::Point> &pointList, int num_points_allowed);      
    };

    namespace regularization {
        // array xyz(array source);
    }

    namespace normalization {
        /**
         * @brief Calculates a new set of timeseries with zero mean and 
         * standard deviation one.
         * 
         * @param tss Expects an input array whose dimension zero is the length of the time 
         * series (all the same) and dimension one indicates the number of 
         * time series.
         * 
         * @param epsilon Minimum standard deviation to consider.  It acts a a gatekeeper for
         * those time series that may be constant or near constant.  
         * 
         * @return array Array with the same dimensions as tss where the time series have been
         * adjusted for zero mean and one as standard deviation.
         */
        array znorm(array tss, double epsilon = 0.00000001);

        /**
         * @brief Adjusts the time series in the given input and performs z-norm 
         * inplace (without allocating further memory)
         * 
         * @param tss Expects an input array whose dimension zero is the length of the time 
         * series (all the same) and dimension one indicates the number of 
         * time series.
         * 
         * @param epsilon Minimum standard deviation to consider.  It acts a a gatekeeper for
         * those time series that may be constant or near constant.  
         */
        void znormInPlace(array &tss, double epsilon = 0.00000001);

        /**
         * @brief Normalizes the given time series according to its minimum and maximun value and 
         * adjusts each value within the range [low, high]
         * 
         * @param tss Expects an input array whose dimension zero is the length of the time 
         * series (all the same) and dimension one indicates the number of 
         * time series.
         * 
         * @param high Maximum final value.  Defaults to 1.0
         * @param low  Minimum final value.  Defaults to 0.0
         * 
         * @param epsilon Safeguard for constant (or near constant) time series as the operation implies
         * a unit scale operation between min and max values in the tss.
         * 
         * @return array An array with the same dimensions as tss, whose values (time series in dimension 0)
         * have been normalized by maximun and minimun values, and scaled as per high and low parameters.
         */
        array maxMinNorm(array tss, double high = 1.0, double low = 0.0, double epsilon = 0.00000001);

        /**
         * @brief Same as maxMinNorm, but it performs the operation in place, without allocating further memory.
         * 
         * @param tss Expects an input array whose dimension zero is the length of the time 
         * series (all the same) and dimension one indicates the number of 
         * time series.
         * 
         * @param high Maximum final value.  Defaults to 1.0
         * @param low  Minimum final value.  Defaults to 0.0
         * 
         * @param epsilon Safeguard for constant (or near constant) time series as the operation implies
         * a unit scale operation between min and max values in the tss.
         */
        void maxMinNormInPlace(array &tss, double high = 1.0, double low = 0.0, double epsilon = 0.00000001);


        array decimalScalingNorm(array tss);

        void decimalScalingNormInPlace(array &tss);

        // decimal scaling
        // adaptive normalization
    };

    namespace features {
        // tsfresh
        // PIP Perceptualy important points
    };

    namespace dimensionality {
        // PAA/PLA
        // SAX
        // Principal Components A.
        // Random Projections
    };

    namespace distances {

        /**
         * @brief Calculates euclidian distances between timeseries.  
         * 
         * @param tss Expects an input array whose dimension zero is the length of the time 
         * series (all the same) and dimension one indicates the number of 
         * time series.
         * 
         * @return array Returns an upper triangular matrix where each position corresponds to the 
         * distance between two time series.  Diagonal elements will be zero.  For example: 
         * Position row 0 column 1 will record the distance between time series 0 
         * and time series 1.
         */
        array euclidian(array tss);

        /**
         * @brief Calculates non squared version of the euclidian distance.
         * 
         * @param tss Expects an input array whose dimension zero is the length of the time 
         * series (all the same) and dimension one indicates the number of 
         * time series.
         * 
         * @return array Returns an upper triangular matrix where each position corresponds to the 
         * distance between two time series.  Diagonal elements will be zero.  For example: 
         * Position row 0 column 1 will record the distance between time series 0 
         * and time series 1.
         */
        array squaredEuclidian(array tss);

        // habituales + la ncc

        // una funcion que dada un tipo de destiancia retorna 
        // una matriz triangular de distancias entre timeseries 
    };

    namespace matrix {
        /**
         * @brief Calculates the sliding dot product of the time series 'q' against t
         * 
         * @param q Array whose first dimension is the length of the query time series
         * and the last dimension is the number of time series to calculate
         * @param t Array with the second time series in the first dimension
         * @return array Returns an array with as many elements as 't' in the first dimension
         * and as many elements as the last dimension of 'q' in the last dimension
         */
        array slidingDotProduct(array q, array t);

        /**
         * @brief Calculates the moving average and standard deviation of the time series 't'.
         * 
         * @param t Input time series. Multiple time series 
         * @param a Auxiliary array to be used in the function calculateDistanceProfile. Use
         * the overloaded method without this parameter
         * @param m Window size
         * @param mean Output array containing the moving average
         * @param stdev Output array containing the moving standard deviation
         */
        void meanStdev(af::array t, af::array *a, long m, af::array *mean, af::array *stdev);

        /**
         * @brief Calculates the moving average and standard deviation of the time series 't'.
         * 
         * @param t Input time series. Multiple time series 
         * @param m Window size
         * @param mean Output array containing the moving average
         * @param stdev Output array containing the moving standard deviation
         */
        void meanStdev(af::array t, long m, af::array *mean, af::array *stdev);
        
        /**
         * @brief Calculates the distance between 'q' and the time series 't', which produced the sliding. Multiple queries can
         * be computed simultaneously in the last dimension of 'q'.
         * @param qt The sliding dot product of 'q' and 't'
         * @param a Auxiliary array computed using the meanStdev function. This array contains a
         * precomputed fixed value to speed up the distance calculation 
         * @param sum_q Sum of the values contained in 'q'
         * @param sum_q2 Sum of squaring the values contained in 'q'
         * @param mean_t Moving average of 't' using a window size equal to the number of elements
         * in 'q'
         * @param sigma_t Moving standard deviation of 't' using a window size equal to the number of elements
         * in 'q'
         * @param distance Resulting minimal distance
         * @param index Position where the minimum is occurring
         */
        void calculateDistanceProfile(af::array qt, af::array a,
                                af::array sum_q, af::array sum_q2, af::array mean_t, af::array sigma_t,
                                af::array *distance, af::array *index);

        /**
         * @brief 
         * 
         * @param q Array whose first dimension is the length of the query time series
         * and the last dimension is the number of time series to calculate
         * @param t Array with the second time series in the first dimension
         * @param a Auxiliary array computed using the meanStdev function. This array contains a
         * precomputed fixed value to speed up the distance calculation
         * * @param mean_t Moving average of 't' using a window size equal to the number of elements
         * in 'q'
         * @param sigma_t Moving standard deviation of 't' using a window size equal to the number of elements
         * in 'q'
         * @param distance Resulting minimal distance
         * @param index Position where the minimum is occurring
         */
        void mass(array q, array t, array a, array mean_t, array sigma_t, array *distance, array *index);

        /**
         * @brief STAMP algorithm to calculate the matrix profile between 'ta' and 'tb' using asubsequence length
         * of 'm'
         * 
         * @param ta Query time series
         * @param tb Reference time series
         * @param m Subsequence length
         * @param profile The matrix profile, which reflects the distance to the closer element of the subsequence
         * from 'ta' in 'tb'
         * @param index The matrix profile index, which points to where the previously mentioned minimum is located
         */
        void stamp(array ta, array tb, long m, af::array *profile, af::array *index);
    };
};

