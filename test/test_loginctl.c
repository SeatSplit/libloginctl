#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <systemd/sd-login.h>
#include <glib.h>

#include "loginctl.h"

void print_seat_usernames() {
    char **seats;

    const int seat_count = sd_get_seats(&seats);
    if (seat_count < 0) {
        fprintf(stderr, "Failed to get seats: %s\n", strerror(-seat_count));
        return;
    }

    printf("Number of seats: %d\n", seat_count);
    for (int i = 0; i < seat_count; ++i) {
        char **sessions;

        printf("Seat %d: %s\n", i, seats[i]);

        const int session_count = sd_seat_get_sessions(seats[i], &sessions, NULL, NULL);
        if (session_count < 0) {
            fprintf(stderr, "Failed to get sessions for seat %s: %s\n", seats[i], strerror(-session_count));
            continue;
        }

        for (int j = 0; j < session_count; ++j) {
            char *username = NULL;

            if (sd_session_get_username(sessions[j], &username) == 0 && username) {
                printf("  Session %d: %s (username: %s)\n", j, sessions[j], username);
                free(username);
            }

            uid_t *uid = malloc(sizeof(uid_t));

            if (!uid) {
                fprintf(stderr, "Failed to allocate memory for uid\n");
                continue;
            }

            if (sd_session_get_uid(sessions[j], uid) == 0 && *uid) {
                printf("  Session %d: %s (uid: %jd)\n", j, sessions[j], (intmax_t) *uid);
                free(uid);
            }

            char *seat = NULL;

            if (sd_session_get_seat(sessions[j], &seat) == 0 && seat) {
                printf("  Session %d: %s (seat: %s)\n", j, sessions[j], seat);
            }

            char *tty = NULL;

            if (sd_session_get_tty(sessions[j], &tty) == 0 && tty) {
                printf("  Session %d: %s (tty: %s)\n", j, sessions[j], tty);
            }

            char *state = NULL;

            if (sd_session_get_state(sessions[j], &state) == 0) {
                printf("  Session %d: %s (state: %s)\n", j, sessions[j], state);
            }
        }

        g_strfreev(sessions);
    }

    g_strfreev(seats);
}

int main() {
    print_seat_usernames();
    Session *sessions = NULL;
    int num_sessions = loginctl_get_sessions(&sessions);

    for (int i = 0; i < num_sessions; i++) {
        printf("Session %d: %s\n", i, sessions[i].session);
        printf("  UID: %d\n", sessions[i].uid);
        printf("  Username: %s\n", sessions[i].username);
        printf("  TTY: %s\n", sessions[i].tty);
        printf("  State: %s\n", sessions[i].state);
    }

    return 0;
}
