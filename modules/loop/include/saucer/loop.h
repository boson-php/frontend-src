#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"
#include "export.h"

#ifndef SAUCER_LOOP_EXPORT
#define SAUCER_LOOP_EXPORT SAUCER_EXPORT
#endif

    struct saucer_loop;

    SAUCER_LOOP_EXPORT void saucer_loop_free(saucer_loop *);
    SAUCER_LOOP_EXPORT saucer_loop *saucer_loop_new(saucer_application *);

    SAUCER_LOOP_EXPORT void saucer_loop_run(saucer_loop *);
    SAUCER_LOOP_EXPORT void saucer_loop_iteration(saucer_loop *);

    SAUCER_LOOP_EXPORT void saucer_loop_quit(saucer_loop *);

#ifdef __cplusplus
}
#endif
