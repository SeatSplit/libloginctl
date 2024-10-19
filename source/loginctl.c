#ifndef LOGINCTL_C
#define LOGINCTL_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <systemd/sd-login.h>
#include <glib.h>
#include "loginctl.h"

int loginctl_get_sessions(Session **sessions) {
    char **session_ids = NULL;
    const int session_count = sd_get_sessions(&session_ids);

    if (session_count < 0) {
        return -1; // Return -1 if the session count is negative
    }

    if (session_count == 0) {
        *sessions = NULL;
        return 0; // If no sessions are found, the function will return 0 and `sessions` will remain `NULL`.
    }

    // Allocate memory for sessions
    *sessions = (Session *) calloc(session_count, sizeof(Session));

    if (*sessions == NULL) {
        g_strfreev(session_ids);
        // If memory allocation fails, returns error code
        return -ENOMEM;
    }

    int valid_sessions = 0;

    for (int i = 0; i < session_count; i++) {
        Session *session = &(*sessions)[valid_sessions];

        session->session = strdup(session_ids[i]);
        if (session->session == NULL) {
            goto cleanup;
        }

        if (sd_session_get_uid(session->session, &session->uid) < 0) {
            session->uid = (uid_t) -1;
        }

        if (sd_session_get_username(session->session, &session->username) < 0) {
            session->username = NULL;
        }

        if (sd_session_get_tty(session->session, &session->tty) < 0) {
            session->tty = NULL;
        }

        if (sd_session_get_state(session->session, &session->state) < 0) {
            session->state = NULL;
        }

        valid_sessions++;
    }

    g_strfreev(session_ids);
    return valid_sessions;
// Cleanup on error
    cleanup:
    for (int j = 0; j < valid_sessions; j++) {
        free((*sessions)[j].session);
        free((*sessions)[j].username);
        free((*sessions)[j].tty);
        free((*sessions)[j].state);
    }

    free(*sessions);
    *sessions = NULL;
    g_strfreev(session_ids);
    return -ENOMEM;
    //
}
#endif // LOGINCTL_C
