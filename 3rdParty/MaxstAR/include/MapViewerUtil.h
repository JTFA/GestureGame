/*==============================================================================
Copyright 2017 Maxst, Inc. All Rights Reserved.
==============================================================================*/

#if !defined(__IOS__)
#include "Types.h"

namespace maxstAR
{
	class MAXSTAR_API MapViewerUtil
	{
	public:
		MapViewerUtil();
		~MapViewerUtil();

		static MapViewerUtil* getInstance();

		bool initialize(const char *fileName);

		void deinitialize();

		int createTriangle(int index);

		void getIndices(int * indexBuffer);

		void getTexCoords(float *texCoords);

		const char *getJson();

		unsigned char *getImage(int idx);

		int getImageSize(int idx);
	};
}
#endif // __IOS__
