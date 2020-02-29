#include <stdio.h>
#include <string>
#include <memory>

using namespace std;

class SystemUtil {
public:
	static shared_ptr<SystemUtil> getInstance();
	~SystemUtil();

	void setFlipHorizontal(bool toggle);
	void setFlipVertical(bool toggle);
	bool isFlipHorizontal();
	bool isFlipVertical();
	void clear();

private:
	static shared_ptr<SystemUtil> instance;

	SystemUtil();

	bool backgroundFlipHorizontal = false;
	bool backgroundFlipVertical = false;
};
