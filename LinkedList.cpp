// LinkedSequence.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include <iostream>
#include "Sequence.h"
template<typename Key, typename Info>
Sequence<Key, Info>merge(const Sequence<Key, Info>& s1, int start1, int len1, const Sequence<Key, Info>& s2, int start2, int len2, int count)
{
	if (count < 0)
		throw std::logic_error("Cannot copy less than 0 times");
	if (len1 < 0 || len2 < 0)
		throw std::logic_error("Length cannot be less than 0");
	if (start1 < 0 || start2 < 0)
		throw std::logic_error("Cannot skip less than 0 elements");
	Sequence<Key, Info> seq;
	auto it1 = s1.begin() + start1;
	auto it2 = s2.begin() + start2;
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < len1; j++)
		{
			seq.concatenate(s1.copy(it1));
			it1++;
		}

		for (int j = 0; j < len2; j++)
		{
			seq.concatenate(s2.copy(it2));
			it2++;
		}
	}
	return seq;
}


template<typename Key, typename Info>
void print(Sequence<Key, Info> seq, int seq_num)
{
	if (seq.empty())
	{
		std::cout << "An empty sequence" << std::endl;
	}
	else
	{
		std::cout << "Sequence " <<seq_num<< std::endl;
		auto it = seq.begin();
		std::cout << "Key \tInfo" << std::endl;
		for (; it != seq.end(); it++)
		{
			std::cout << it.key() << "\t" << it.info() << std::endl;
		}
		std::cout <<"-------------------------------------------------"<< std::endl;
	}
}

template<typename Key, typename Info>
void print(Sequence<Key, Info> seq, Sequence<Key, Info> seq2, int seqn1,int seqn2)
{
	auto it = seq.begin();
	auto it2 = seq2.begin();
	std::cout << "Sequence "<<seqn1<<" \tSequence "<<seqn2<< std::endl;
	std::cout << "Key \tInfo \tKey \tInfo" << std::endl;
	for (; it != seq.end() || it2!=seq2.end(); it++,it2++)
	{
		if (it != seq.end())
			std::cout << it.key() << "\t" << it.info() << "\t";
		if (it2 != seq2.end() && it == seq.end())
			std::cout << "\t" << "\t" << it2.key() << "\t" << it2.info();
		else if (it2 != seq2.end())
			std::cout  << it2.key() << "\t" << it2.info();
		std::cout << std::endl;
	}
	std::cout << "-------------------------------------------------" << std::endl;

}


int main()
{
	//push back
	Sequence<int, char> Seq1;
	{
		int i = 0;
		char c = 'a';
		for (; i < 10; i++, c++)
		{
			Seq1.push_back(i, c);
		}
	}

	print(Seq1 + Seq1, 1);
	//Sequence1.erase(Sequence1.begin(), Sequence1.begin() + 3);
	Sequence<int, char> Seq2 = Seq1; //test =
	Seq2.push_back(200, 'c');
	Seq2.erase(Seq2.begin() + 2, Seq2.begin()+4);//remove 2nd-4th element (counting from 0)
	print(Seq1,Seq2,1,2);
	
	auto it = Seq2.begin();
	it = Seq2.insert(it + 2, 5, 5, 'p'); //test insert duplicates
	print(Seq2,2);
	
	Sequence<int, char> Seq3(Seq2); //test copyconstructor
	while (Seq3.erase_key(5)); //erase all nodes with key 5;
	print(Seq2, Seq3, 2, 3);

	Seq3.clear();
	//Seq3.at(2); will thow an exeption out of range
	if (Seq3.contains(3))
		std::cout << "Key found" << std::endl;
	else
		std::cout << "No key" << std::endl;
	Seq3.insert(Seq3.begin() + 2000, 2,'c');
	Seq3.swap(Seq2); //test swap
	print(Seq2, Seq3,2,3);

	Seq2.concatenate(Seq3);
	print(Seq2,2);
	Seq2 = Seq1;
	Seq2.pop_back();
	Seq2.back().key = 1337;
	Seq2.back().info = 'z';
	Seq2.front().key = 2222;
	Seq2.front().info = 'a';
	print(Seq1, Seq2,1,2);
	Seq1.erase(Seq1.begin(), Seq1.end()); //similar to clear()
	print(Seq1, 1);
	Seq1 += Seq2; //+=
	print(Seq1, 1);
	//Seq1.erase(Seq1.begin() + 2, Seq1.begin()); //throws an exeption logic error
	Seq1.erase(Seq1.begin() + 3, Seq1.end());
	print(Seq1, 1);
	//Seq1[20].key = 0;  will thow an exeption out of range
	Seq1[-1].key = 777;//change last element
	Seq1[1].key = 500;
	print(Seq1, 1);
	Seq1.reverse();//test of reverse
	print(Seq1, 1);
	Seq1 += Seq2.copy(Seq2.begin() + 2, Seq2.end());//test copy
	print(Seq1, 1);
	{
		Sequence<int, char> Seq4(Seq2.begin() + 3, Seq2.end()); //test copyconstructor
		print(Seq4, 4);
	}
	auto nw = Seq1[2];
	nw = Seq1[0]; //test = of nodewrapper
	nw.key = 1111111;
	print(Seq1, 1);
	auto it2 = Seq1.insert(Seq1.begin()+2,5, Seq1[2]); //test of insert return value
	Seq1.insert(it2 , 2, Seq1[-1]);
	print(Seq1, 1);


	print(Seq1, Seq2,1,2);
	Seq3 = merge(Seq1, 2, 3, Seq2, 1, 2, 4); //test merge
	print(Seq3,3);

	Seq3.clear();
	Seq3 = merge(Seq1, 200, 3, Seq2, 100, 2, 4); //test merge
	print(Seq3, 3);
	Seq1.clear();
	Seq3 = merge(Seq1, 2, 30, Seq2, 0, 2, 4); //test merge
	print(Seq3, 3);
	return 0;


}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
