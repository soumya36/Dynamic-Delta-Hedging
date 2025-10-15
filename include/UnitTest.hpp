#pragma once

// Lightweight test macros — users should include <iostream> or <cmath> if needed by the
// expressions passed to these macros. Keeping this header minimal avoids transitive
// includes in consumer files.

#define EXPECT_NEAR(a,b,eps) \
	do { \
		if (std::fabs((a) - (b)) > (eps)) { \
			std::cout << "[FAIL] " << __FILE__ << ":" << __LINE__ << " got " << (a) << " expected ~" << (b) << "\n"; \
		} else { \
			std::cout << "[PASS] EXPECT_NEAR\n"; \
		} \
	} while(0)

#define EXPECT_TRUE(cond) \
	do { \
		if (!(cond)) { \
			std::cout << "[FAIL] " << __FILE__ << ":" << __LINE__ << " condition failed\n"; \
		} else { \
			std::cout << "[PASS] EXPECT_TRUE\n"; \
		} \
	} while(0)
