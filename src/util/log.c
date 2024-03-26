/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2024, Jacob Gibson
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "util/log.h"
#include "config.h"

#ifdef USE_SYSLOG
#include <syslog.h>
#endif

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>



void logger_init(struct logger *l)
{
        if (l->init) return;
        if (l->log_file) {
                if (l->fp != NULL) return;
                l->fp = fopen(l->log_path, "w");
                if (l->fp == NULL) {
                        perror("fopen");
                        return;
                }
        }

        #ifdef USE_SYSLOG
        setlogmask(LOG_UPTO (LOG_NOTICE));
        openlog ("hephaestus", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
        #endif

        l->init = true;

        printlog(LINFO, "Initialized logger successfully", l);

        return;
}

void logger_quit(struct logger *l)
{
        l->init = false;
        if (l->log_file) {
                if (l->fp == NULL) return;
                fclose(l->fp);
                l->fp = NULL;
        }
        return;
}

void printlog(const int lvl, const char *msg, struct logger *l)
{
        if (!l->init)
                return;
        #ifdef NDEBUG
        if (lvl == LDEBUG)
                return;
        #endif

        #ifdef USE_SYSLOG
        int priority = LOG_INFO;
        #endif
        char t_buff[26] = {0};
        struct tm *t_info = NULL;
        char lvl_str[8] = {0};
        FILE *print_out = stdout;
        time_t t = time(NULL);
 
        t_info = localtime(&t);
        strftime(t_buff, 26, "%Y-%m-%d %H:%M:%S", t_info); 

        switch (lvl) {
                case LDEBUG:
                        #ifdef USE_SYSLOG
                        priority = LOG_DEBUG;
                        #endif
                        strncpy(lvl_str, "DEBUG", sizeof(lvl_str));
                        break;
                case LWARN:
                        print_out = stderr;
                        #ifdef USE_SYSLOG
                        priority = LOG_WARNING;
                        #endif
                        strncpy(lvl_str, "WARNING", sizeof(lvl_str));
                        break;
                case LERROR:
                        print_out = stderr;
                        #ifdef USE_SYSLOG
                        priority = LOG_ERR;
                        #endif
                        strncpy(lvl_str, "ERROR", sizeof(lvl_str));
                        break;
                case LFATAL:
                        #ifdef USE_SYSLOG
                        syslog(LOG_CRIT, "%s", msg);
                        closelog();
                        #endif
                        fprintf(stderr, "%s FATAL: %s\n", t_buff, msg);
                        if (l->log_file) {
                                fprintf(l->fp, "%s FATAL: %s\n", t_buff, msg);
                                fclose(l->fp);
                        }
                        abort();
                default:
                        strncpy(lvl_str, "INFO", sizeof(lvl_str));
                        break;
        }

        #ifdef USE_SYSLOG
        syslog(priority, "%s", msg);
        #endif
        fprintf(print_out, "%s %s: %s\n", t_buff, lvl_str, msg);
        if (l->log_file)
                fprintf(l->fp, "%s %s: %s\n", t_buff, lvl_str, msg);

        return;
}

void errnolog(const int lvl, struct logger *l)
{
        printlog(lvl, strerror(errno), l);

        return;
}

