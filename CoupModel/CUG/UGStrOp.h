
class UGStr
{
public:
	static char* fcvt(double val, int count, int * dec, int * sign);

	static void strncpy(char * dest, size_t size, const char * src, size_t count);

	static void stprintf(TCHAR * dest, size_t size, const TCHAR * src, ...);

	static void tcscat(TCHAR * dest, SIZE_T length, const TCHAR* src);

	static void tcsncat(TCHAR * dest, size_t size, const TCHAR * src, size_t count);

	static void tcscpy(TCHAR * dest, SIZE_T length, const TCHAR* src);
};