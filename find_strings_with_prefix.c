/* find_strings_with_prefix.c
 *
 * Find all strings in a given list of strings that start with a given prefix.
 * Uses GLib.
 *
 * COMPILE TEST
 *
 * gcc `pkg-config --cflags glib-2.0` -o test_find_strings_with_prefix find_strings_with_prefix.c `pkg-config --libs glib-2.0` -Dtest_find_strings_with_prefix
 *
 * TEST
 *
 * ./test_find_strings_with_prefix
 */

#ifdef test_find_strings_with_prefix
#include <glib-2.0/glib.h>
#endif

/* Used with foreach. Add a string to a list if the string contains a given
 * substring.
 *
 * data - The string we want to search through (the "haystack").
 *
 * user_data - A pointer to a list pointer (so that the old pointer can be
 * updated if needed). The list initially contains one item, the string
 * we want to find (the "needle"). A string containing the needle is 
 * appended to the list each time this function is called by foreach.
 *
 */
static void append_if_match_cb( gpointer data, gpointer user_data )
{
	gchar *haystack = (gchar *) data;
	GList **list_ptr = (GList **) user_data;

	g_assert( list_ptr );

	/* The first string in the list is the string we want to search for,
	 * i.e. the "needle".
	 */
	GList *first = g_list_first( *list_ptr );
	gchar *needle = (gchar *) first->data;

	/* Matches are appended to the end of the list.
	 */
	gssize haystack_len = -1; // search the whole string, like strstr

	if ( g_strstr_len( haystack, haystack_len, needle ) )
		*list_ptr = g_list_append( *list_ptr, haystack );
}

/* Find a matching string ("needle") in a list of strings ("haystacks").
 *
 * haystacks - list of strings to search for needle
 * needle - string to find in haystacks
 */
GList *find_strings_with_prefix( GList *haystacks, gchar *needle )
{
	/* Create a list with the needle as the only element, like
	 * append_if_match expects.
	 */
	GList *found = NULL;

	/* Append the needle to the empty list, like append_if_match_cb expects.
	 */
	found = g_list_append( found, needle );

	/* Find any matching strings in the list of strings, and append them to
	 * the list of found strings.
	 */
	g_list_foreach( haystacks, append_if_match_cb, &found );

	/* Remove the first element, which is the search query.
	 */
	found = g_list_remove( found, (gconstpointer) needle );

	return found;
}

#ifdef test_find_strings_with_prefix
int main()
{
	GList *string_list = NULL, *found = NULL;

	string_list = g_list_append( string_list, "abc" );
	string_list = g_list_append( string_list, "def" );
	string_list = g_list_append( string_list, "abc" );
	string_list = g_list_append( string_list, "def" );

	gchar* target_string = "ab";

	found = find_strings_with_prefix( string_list, target_string );

	g_assert( g_list_length( found ) == 2 );

	return 0;
}
#endif
