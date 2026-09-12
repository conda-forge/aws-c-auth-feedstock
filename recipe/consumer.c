#include <aws/auth/auth.h>
#include <aws/auth/credentials.h>
#include <stdio.h>
#include <string.h>

static int equals(struct aws_byte_cursor value, const char *expected) {
    return value.len == strlen(expected) && !memcmp(value.ptr, expected, value.len);
}
int main(void) {
    struct aws_allocator *allocator = aws_default_allocator();
    aws_auth_library_init(allocator);
    /* Synthetic data: no external credentials or service access. */
    struct aws_credentials *credentials = aws_credentials_new(
        allocator, aws_byte_cursor_from_c_str("test-access-id"),
        aws_byte_cursor_from_c_str("test-secret"), aws_byte_cursor_from_c_str("test-token"), 1234567890);
    int result = 1;
    if (!credentials) goto cleanup;
    if (!equals(aws_credentials_get_access_key_id(credentials), "test-access-id") ||
        !equals(aws_credentials_get_secret_access_key(credentials), "test-secret") ||
        !equals(aws_credentials_get_session_token(credentials), "test-token") ||
        aws_credentials_get_expiration_timepoint_seconds(credentials) != 1234567890) goto cleanup;
    puts("Installed credential construction and field checks passed");
    result = 0;
cleanup:
    if (credentials) aws_credentials_release(credentials);
    aws_auth_library_clean_up();
    return result;
}
