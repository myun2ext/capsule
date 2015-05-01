#ifndef __MYUN2_GITHUB_COM__CAPSULE__LOGGER_HPP__
#define __MYUN2_GITHUB_COM__CAPSULE__LOGGER_HPP__

#include <time.h>
#include <stdio.h>

namespace myun2
{
	namespace capsule
	{
		class logger
		{
			FILE* fp;
			bool is_file;
			const unsigned long rotate_size;
			const unsigned int rotate_count;
		public:
			logger(FILE* _fp) : is_file(false), fp(_fp), rotate_size(0), rotate_count(0) {}
			logger(const char* name, unsigned long _rotate_size = 10 * 1024 * 1024, unsigned int _rotate_count = 3)
				: is_file(true), rotate_size(_rotate_size), rotate_count(_rotate_count) { fp = fopen(name, "a"); }

			void log(const char* s)
			{
				time_t t = time(NULL);
    			struct tm *l = localtime(&t);

				fprintf(fp,
					"%04d/%02d/%02d %02d:%02d:%02d %s\n",
			    	l->tm_year + 1900, l->tm_mon + 1, l->tm_mday,
			 		l->tm_hour, l->tm_min, l->tm_sec, s);
			 	//if ( ftell(fp) > _rotate_size ) rotate();
			}
		};
	}
}

#endif//__MYUN2_GITHUB_COM__CAPSULE__LOGGER_HPP__
