
#include "libchained/chained.h"
#include <sys/resource.h> /* For rlimit stuff */

/**
 * @brief I try to make sure we're dropping cores...
 * This function attempts to make sure we drop cores.
 * Should it fail, it will generate an error to the stderr.
 * @todo
 * Ideally, we need some way of making error here fatal.
 * That way we can get the program to halt if it won't be able to drop cores.
 * @note
 * Thankie w00t for pointing this out :)
 */

void cis_drop_cores (void)
{
	struct rlimit rlim;
	memset(&rlim,0,sizeof(rlim));
	if (!getrlimit(RLIMIT_CORE,&rlim)) {
		rlim.rlim_cur = rlim.rlim_max;
		if (setrlimit(RLIMIT_CORE,&rlim)) {
			perror("rlimit");
			fprintf(stderr,"setting the resource limit failed with errno: %d\n",errno);
		}
	}
	return;
}
