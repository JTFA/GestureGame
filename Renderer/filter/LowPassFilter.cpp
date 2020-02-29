
#include <cmath>

#include <Utilities.h>
#include <LogUtils.h>
#include <filter/LowPassFilter.h>

#define COEFFICIENT_OF_DOT 0.99F

using namespace armakers;

LowPassFilter::LowPassFilter()  {

}

LowPassFilter::~LowPassFilter() {
	//LOGD("LowPassFilter Destructor");
}

void LowPassFilter::update(gameplay::Quaternion &prev, gameplay::Quaternion &current, gameplay::Quaternion* out) {
    float dotProd = fabsf(Utilities::dotProduct(prev, current));
    //LOGD("LowPassFilter dotProd (%f)", dotProd);
    gameplay::Quaternion::lerp(prev, current, dotProd < COEFFICIENT_OF_DOT ? 1.F : 0.025F, out);
}