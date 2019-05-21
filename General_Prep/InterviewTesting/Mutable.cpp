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



int main() {

	char a[] = "DawnNews headlines, news stories, updates and latest news from Pakistan. Top political news, bulletins, talk shows, infotainment and much more.";

	std::map<char, int> Counts;

	for (int i = 0; i < strlen(a); i++)
	{
		if(!isSpecialChar(a[i]))
		{
			auto it = Counts.find(a[i]);
			auto it_2 = Counts.begin();
			if(std::isupper(a[i])) 
				it_2 = Counts.find(char(a[i] + ('a' - 'A')));
			else 
				it_2 = Counts.find(char(a[i] - ('a' - 'A')));
			
			if (it == Counts.end() && it_2 == Counts.end())
				Counts[a[i]]=0;
		
			Counts[a[i]]++;
		}
	}

	for (auto i : Counts)
	{
		std::cout << i.first << ": " << i.second << std::endl;
	}
}