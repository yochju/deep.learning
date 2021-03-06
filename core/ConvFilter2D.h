#pragma once

#include "VectorND.h"
#include <ctime>

class ConvFilter2D
{
public:
	int i_res_, j_res_;
	int i_stride_, j_stride_;
	int i_padding_, j_padding_;

	VectorND<F> weights_;
	VectorND<F> delta_weights_; // momentum

	ConvFilter2D()
	{}

	ConvFilter2D(const int& _i_res, const int& _j_res, const int& _i_stride, const int& _j_stride, const int& _i_padding, const int& _j_padding, const F& _rand_scale, const F& _rand_min) {
		initialize(_i_res, _j_res, _i_stride, _j_stride, _i_padding, _j_padding, _rand_scale, _rand_min);
	}

	ConvFilter2D(const ConvFilter2D& fi, const F& _rand_scale, const F& _rand_min) {
		initialize(fi.i_res_, fi.j_res_, fi.i_stride_, fi.j_stride_, fi.i_padding_, fi.j_padding_, _rand_scale, _rand_min);
	}

	void initialize(const ConvFilter2D& fi, const F& _rand_scale, const F& _rand_min) {
		initialize(fi.i_res_, fi.j_res_, fi.i_stride_, fi.j_stride_, fi.i_padding_, fi.j_padding_, _rand_scale, _rand_min);
	}

	void initialize(const int& _i_res, const int& _j_res, const int& _i_stride, const int& _j_stride, const int& _i_padding, const int& _j_padding, const F& _rand_scale, const F& _rand_min) {
		i_res_ = _i_res;
		j_res_ = _j_res;
		i_stride_ = _i_stride;
		j_stride_ = _j_stride;
		i_padding_ = _i_padding;
		j_padding_ = _j_padding;

		weights_.initialize(i_res_* j_res_ + 1); // +1 is for bias
		delta_weights_.initialize(i_res_ * j_res_ + 1);

		//srand((unsigned int)time(0));
		
		for (int i = 0; i < weights_.num_dimension_; ++i) {
			weights_[i] = (F)rand() / (F)RAND_MAX * _rand_scale + _rand_min;
			delta_weights_[i] = (F)0.0;
		}
	}

	int get1DIndex(const int& i, const int& j) const {
		return i + i_res_ * j;
	}

	F& getWeight(const int& i, const int& j) const {
		assert(i >= 0);
		assert(i < i_res_);
		assert(j >= 0);
		assert(j < j_res_);

		return weights_[i + i_res_ * j];
	}

	F& getDeltaWeight(const int& i, const int& j) const {
		assert(i >= 0);
		assert(i < i_res_);
		assert(j >= 0);
		assert(j < j_res_);

		return delta_weights_[i + i_res_ * j];
	}

	F& getBias() const {
		return weights_[weights_.num_dimension_ - 1];	// last component is bias
	}

	F& getDeltaBias() const {
		return delta_weights_[weights_.num_dimension_ - 1];	// last component is bias
	}
};


// end of file
