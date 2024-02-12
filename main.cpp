#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Windows.h>

#pragma execution_character_set("UTF-8")

using namespace std;

class User;

class Post
{
public:
	string title;
	string content;
	Wt::Dbo::ptr<User> author;

	template<typename Action>
	void persist(Action& a)
	{
		Wt:Dbo::field(a, title, "title");
		Wt::Dbo::field(a,content, "content");
		Wt::Dbo::belongsTo(a, author, "user");
	}
};

class User
{
public:
	string name;
	string phone;
	int karma = 0;
	Wt::Dbo::collection<Wt::Dbo::ptr<Post>> posts;

	template<typename Action>
	// Action передаЄтс€ в качестве аргумента в persist
	void persist(Action& a)
	{
		// «аполн€ем пол€
		// «десь написано, какое это поле и как оно называетс€ в базе данных
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::field(a, phone, "phone");
		Wt::Dbo::field(a, karma, "karma");
		Wt:Dbo::hasMany(a, posts, Wt::Dbo::ManyToOne, "user");
	}
};

int main()
{
	//setlocale(LC_ALL, "Russian");

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	cout << "Hello world" << endl;
	
	try
	{
		string connectionString =
			"host = 127.0.0.1 "
			"port = 5432 "
			"dbname = lesson06 "
			"user = postgres "
			"password = 111111";

		auto connection = make_unique<Wt::Dbo::backend::Postgres>(connectionString);

		Wt::Dbo::Session session;
		session.setConnection(std::move(connection));
		session.mapClass<User>("user");
		session.mapClass<Post>("post");

		try
		{
			session.createTables();
		}
		catch (const exception& e)
		{
			cout << "Tables already exists, sipping ... " << endl;
		}
		
		Wt::Dbo::Transaction t(session);

	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}
	
	return 0;
}
