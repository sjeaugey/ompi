/*
 * Copyright (c) 2010      Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2012      Los Alamos National Security, Inc. All rights reserved.
 * Copyright (c) 2014-2017 Intel, Inc.  All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */


#include <src/include/pmix_config.h>
#include <pmix_common.h>

#include "src/util/error.h"

#include "src/mca/psensor/base/base.h"

static bool mods_active = false;

pmix_status_t pmix_psensor_base_start(pmix_peer_t *requestor, pmix_status_t error,
                                      const pmix_info_t *monitor,
                                      const pmix_info_t directives[], size_t ndirs)
{
    pmix_psensor_active_module_t *mod;
    pmix_status_t rc;

    opal_output_verbose(5, pmix_psensor_base_framework.framework_output,
                        "%s:%d sensor:base: starting sensors",
                        pmix_globals.myid.nspace, pmix_globals.myid.rank);

    /* call the start function of all modules in priority order */
    PMIX_LIST_FOREACH(mod, &pmix_psensor_base.actives, pmix_psensor_active_module_t) {
        if (NULL != mod->module->start) {
            rc = mod->module->start(requestor, error, monitor, directives, ndirs);
            if (PMIX_SUCCESS != rc && PMIX_ERR_TAKE_NEXT_OPTION != rc) {
                return rc;
            }
        }
    }

    return PMIX_SUCCESS;
}

pmix_status_t pmix_psensor_base_stop(pmix_peer_t *requestor,
                                     char *id)
{
    pmix_psensor_active_module_t *mod;
    pmix_status_t rc;

    opal_output_verbose(5, pmix_psensor_base_framework.framework_output,
                        "%s:%d sensor:base: stopping sensors",
                        pmix_globals.myid.nspace, pmix_globals.myid.rank);

    /* call the stop function of all modules in priority order */
    PMIX_LIST_FOREACH(mod, &pmix_psensor_base.actives, pmix_psensor_active_module_t) {
        if (NULL != mod->module->stop) {
            rc = mod->module->stop(requestor, id);
            if (PMIX_SUCCESS != rc && PMIX_ERR_TAKE_NEXT_OPTION != rc) {
                return rc;
            }
        }
    }

    return PMIX_SUCCESS;
}
