#include "txbase_tests/helper.h"
#include "txbase/math/color.h"

namespace TX
{
	namespace Tests
	{
		TEST(ColorTests, ClampTest){
			EXPECT_EQ(Color(+.5f, +.5f, +.5f), Color(+.5f, +.5f, +.5f).Clamp());
			EXPECT_EQ(Color(+1.f, +1.f, +1.f), Color(+1.f, +1.f, +1.f).Clamp());
			EXPECT_EQ(Color(+0.f, +0.f, +0.f), Color(+0.f, +0.f, +0.f).Clamp());
			EXPECT_EQ(Color(+0.f, +0.f, +0.f), Color(-1.f, -1.f, -1.f).Clamp());
			EXPECT_EQ(Color(+1.f, +1.f, +1.f), Color(+2.f, +2.f, +2.f).Clamp());
			EXPECT_EQ(Color(+.5f, +1.f, +1.f), Color(+.5f, +2.f, +2.f).Clamp());
			EXPECT_EQ(Color(+1.f, +.5f, +1.f), Color(+2.f, +.5f, +2.f).Clamp());
			EXPECT_EQ(Color(+1.f, +1.f, +.5f), Color(+2.f, +2.f, +.5f).Clamp());
		}

	}
}
