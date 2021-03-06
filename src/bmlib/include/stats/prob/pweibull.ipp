/*################################################################################
  ##
  ##   Copyright (C) 2011-2018 Keith O'Hara
  ##
  ##   This file is part of the StatsLib C++ library.
  ##
  ##   Licensed under the Apache License, Version 2.0 (the "License");
  ##   you may not use this file except in compliance with the License.
  ##   You may obtain a copy of the License at
  ##
  ##       http://www.apache.org/licenses/LICENSE-2.0
  ##
  ##   Unless required by applicable law or agreed to in writing, software
  ##   distributed under the License is distributed on an "AS IS" BASIS,
  ##   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  ##   See the License for the specific language governing permissions and
  ##   limitations under the License.
  ##
  ################################################################################*/

/*
 * cdf of the Weibull distribution
 */

//
// single input

template<typename T>
statslib_constexpr
T
pweibull_int(const T x, const T shape_par, const T scale_par)
{
    return ( T(1.0) - stmath::exp(-stmath::pow(x,shape_par)) );
}

template<typename T>
statslib_constexpr
T
pweibull(const T x, const T shape_par, const T scale_par, const bool log_form)
{
    return ( ( (shape_par < STLIM<T>::epsilon()) || (scale_par < STLIM<T>::epsilon()) ) ? STLIM<T>::quiet_NaN() :
             //
             x < STLIM<T>::epsilon() ? (log_form == false ? T(0.0) : - STLIM<T>::infinity()) :
             //
             log_form == false ? pweibull_int(x/scale_par,shape_par,scale_par) : 
                                 stmath::log(pweibull_int(x/scale_par,shape_par,scale_par)) );
}

//
// matrix/vector input

template<typename Ta, typename Tb, typename Tc>
void
pweibull_int(const Ta* __stats_pointer_settings__ vals_in, const Tb shape_par, const Tb scale_par, const bool log_form, 
                   Tc* __stats_pointer_settings__ vals_out, const uint_t num_elem)
{
#ifdef STATS_USE_OPENMP
    #pragma omp parallel for
#endif
    for (uint_t j=0U; j < num_elem; j++)
    {
        vals_out[j] = pweibull(vals_in[j],shape_par,scale_par,log_form);
    }
}

#ifdef STATS_USE_ARMA
template<typename Ta, typename Tb, typename Tc>
ArmaMat<Tc>
pweibull(const ArmaMat<Ta>& X, const Tb shape_par, const Tb scale_par, const bool log_form)
{
    ArmaMat<Tc> mat_out(X.n_rows,X.n_cols);

    pweibull_int<Ta,Tb,Tc>(X.memptr(),shape_par,scale_par,log_form,mat_out.memptr(),mat_out.n_elem);

    return mat_out;
}
#endif

#ifdef STATS_USE_BLAZE
template<typename Ta, typename Tb, typename Tc, bool To>
BlazeMat<Tc,To>
pweibull(const BlazeMat<Ta,To>& X, const Tb shape_par, const Tb scale_par, const bool log_form)
{
    BlazeMat<Tc,To> mat_out(X.rows(),X.columns());

    pweibull_int<Ta,Tb,Tc>(X.data(),shape_par,scale_par,log_form,mat_out.data(),X.rows()*X.columns());

    return mat_out;
}
#endif

#ifdef STATS_USE_EIGEN
template<typename Ta, typename Tb, typename Tc, int iTr, int iTc>
EigMat<Tc,iTr,iTc>
pweibull(const EigMat<Ta,iTr,iTc>& X, const Tb shape_par, const Tb scale_par, const bool log_form)
{
    EigMat<Tc,iTr,iTc> mat_out(X.rows(),X.cols());

    pweibull_int<Ta,Tb,Tc>(X.data(),shape_par,scale_par,log_form,mat_out.data(),mat_out.size());

    return mat_out;
}
#endif
