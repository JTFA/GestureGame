#include <SystemUtil.h>

shared_ptr<SystemUtil> SystemUtil::instance = nullptr;

SystemUtil::SystemUtil() {
}

SystemUtil::~SystemUtil() {
}

void SystemUtil::setFlipHorizontal(bool toggle)
{
	backgroundFlipHorizontal = toggle;
}

void SystemUtil::setFlipVertical(bool toggle)
{
	backgroundFlipVertical = toggle;
}

bool SystemUtil::isFlipHorizontal()
{
	return backgroundFlipHorizontal;
}

bool SystemUtil::isFlipVertical()
{
	return backgroundFlipVertical;
}

void SystemUtil::clear()
{
	backgroundFlipHorizontal = false;
	backgroundFlipVertical = false;
}

shared_ptr<SystemUtil> SystemUtil::getInstance() {
	if (instance == nullptr) {
		instance = shared_ptr<SystemUtil>(new SystemUtil());
	}
	return instance;
}
