#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MenuItem
{
public:
	MenuItem(string nm) : m_name(nm) {}
	virtual ~MenuItem() {}
	string name() const { return m_name; }
	virtual bool add(MenuItem* m) = 0;
	virtual const vector<MenuItem*>* menuItems() const = 0;		// the funcion calls "menuItems()" return a pointer of a vector that contains pointers of MenuItem
private:
	string m_name;
};

class PlainMenuItem : public MenuItem   // PlainMenuItem allows no submenus
{
public:
	PlainMenuItem(string nm) : MenuItem(nm) {}
	virtual bool add(MenuItem* m) { return false; }
	virtual const vector<MenuItem*>* menuItems() const { return nullptr; }
};

class CompoundMenuItem : public MenuItem  // CompoundMenuItem allows submenus
{
public:
	CompoundMenuItem(string nm) : MenuItem(nm) {}
	virtual ~CompoundMenuItem();
	virtual bool add(MenuItem* m) { m_menuItems.push_back(m); return true; }
	virtual const vector<MenuItem*>* menuItems() const { return &m_menuItems; }
private:
	vector<MenuItem*> m_menuItems;
};

CompoundMenuItem::~CompoundMenuItem()
{
	for (int k = 0; k < m_menuItems.size(); k++)
		delete m_menuItems[k];
}

void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m->menuItems() == nullptr)	return;
	for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++)
	{
		string submenu = "";
		if (path == "") submenu = (*it)->name();		//if the submenu is not a submenu of aother menu
		else submenu = path + "/" + (*it)->name();		//if the submenu is a submenu of aother menu
		cout << submenu << endl;						//print out the path of the submenu
		listAll((*it), submenu);						//print out the submenu's submenu
	}
}

void listAll(const MenuItem* m)  // one-parameter overload
{
	if (m != nullptr)
		listAll(m, "");
}

int main()
{
	CompoundMenuItem* cm0 = new CompoundMenuItem("New");
	cm0->add(new PlainMenuItem("Window"));
	CompoundMenuItem* cm1 = new CompoundMenuItem("File");
	cm1->add(cm0);
	cm1->add(new PlainMenuItem("Open"));
	cm1->add(new PlainMenuItem("Exit"));
	CompoundMenuItem* cm2 = new CompoundMenuItem("Help");
	cm2->add(new PlainMenuItem("Index"));
	cm2->add(new PlainMenuItem("About"));
	CompoundMenuItem* cm3 = new CompoundMenuItem("");  // main menu bar
	cm3->add(cm1);
	cm3->add(new PlainMenuItem("Refresh"));  // no submenu
	cm3->add(new CompoundMenuItem("Under Development")); // no submenus yet
	cm3->add(cm2);
	listAll(cm3);
	delete cm3;
}