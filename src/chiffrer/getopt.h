#ifndef _GETOPT_PORT_H_
#define _GETOPT_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

extern char *optarg;
extern int optind, opterr, optopt;

int getopt(int argc, char *const argv[], const char *optstring);

#ifdef __cplusplus
}
#endif

#endif /* _GETOPT_PORT_H_ */
