#ifndef __checkfdset_h__
#define __checkfdset_h__

#ifndef _WIN32
#if (FD_SETSIZE < 8192)
#error FD_SETSIZE to small
#endif
#endif

#endif
