// weakPtr.cpp

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <cctype>

std::vector<char> specialchars = { ',', '.','!', '\'', ' ' };
bool isSpecialChar(char r_c)
{
	for (auto c : specialchars)
		if (r_c == c) return true;

	return false;
}


bool hasUniqueChars(char* a, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = i+1; j < n; j++)
			if (*(a + i) == *(a + j)) 
				return false;
	}

	return true;
}

void reverse(char *str) {
	char * end = str;
	char tmp;
	if (str) {
		while (*end) {
			++end;
			
		}
		--end;
		while (str < end) {
			tmp = *str;
			* str++ = *end;
			* end-- = tmp;
			}
		}	
}

void removeDups(char *str) {
	char * idx = str;

	while (idx!='\0')
	{
		
	}
}

int main() {
	char a[] = "apple";

	reverse(a);
	std::cout << "Reversed array: " << a << std::endl;
}

void CountLetters()
{
	char a[] = "DawnNews headlines, news stories, updates and latest news from Pakistan. Top political news, bulletins, talk shows, infotainment and much more.";

	std::map<char, int> Counts;

	for (int i = 0; i < strlen(a); i++)
	{
		if (!isSpecialChar(a[i]))
		{
			if (std::isupper(a[i]))
				a[i] = char(a[i] + ('a' - 'A'));
			auto it = Counts.find(a[i]);


			if (it == Counts.end())
				Counts[a[i]] = 0;

			Counts[a[i]]++;
		}
	}

	for (auto i : Counts)
	{
		std::cout << i.first << ": " << i.second << std::endl;
	}
}