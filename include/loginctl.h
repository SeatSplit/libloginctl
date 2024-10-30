#ifndef LIBLOGINCTL_LIBRARY_H
#define LIBLOGINCTL_LIBRARY_H
#include <sys/types.h>

/**
 * This structure represents a session on the system.
 */
typedef struct Session {
    char *session;
    uid_t uid;
    char *username;
    char *tty;
    char *state;
} Session;

/**
 * Retrieves all active sessions on the system and fills the provided pointer with them.
 *
 * @param sessions A double pointer to an array of Session pointers. The function will
 *                 allocate memory and populate this pointer with active sessions
 *                 found on the system. The caller is responsible for freeing the
 *                 allocated memory once it is no longer needed.
 *
 * @return The number of sessions found on the system. If an error occurs, the function
 *         will return a negative value indicating the error code.
 *
 * @note The `sessions` pointer should be set to `NULL` before calling the function. If
 *       no sessions are found, the function will return 0 and `sessions` will remain `NULL`.
 *
 * @warning The caller must ensure that each element of the array is freed, as well as
 *          the array itself, using `free()` to avoid memory leaks.
 *
 * Example usage:
 *
 * @code
 * Session *sessions = NULL;
 * int num_sessions = loginctl_get_sessions(&sessions);
 * if (num_sessions > 0) {
 *     for (int i = 0; i < num_sessions; i++) {
 *         // Process each session
 *     }
 *     // Free the allocated memory
 *     for (int i = 0; i < num_sessions; i++) {
 *         free(sessions[i]);
 *     }
 *     free(sessions);
 * }
 * @endcode
 */
int loginctl_get_sessions(Session **sessions);


#endif //LIBLOGINCTL_LIBRARY_H
