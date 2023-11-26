#ifndef VECTOR_H
#define VECTOR_H
#include<iostream>
#include<stdexcept>
#include<array>

template<typename T>
//Vector klasse
class Vector
{
public:

	//aliase und so
	class ConstIterator;
	class Iterator;
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;

private:

	//instanz variablen
	size_t sz; //enthaellt die anzahl der im vector gespeicherten elemente
	size_t max_sz; //ist die kapazitaet des vektros
	T* values; //zeigt aufs array
	
public:
	
	//KONSTRUKTOREN UND SO
	
	//erster konstruktor
	//default konstruktor: liefert einen leeren vektor
	//Vector first_vector;
	Vector():
	sz{0},
	max_sz{0},
	values{nullptr}
	{
		
	}
	
	//zweiter konstruktor
	//copy konstruktor
	//Vector second_vector first_vector;
	Vector(const Vector& v): //ka wieso hier "const Vector&" sein muss. der compiler hat mir gesagt ich soll es so machen
	sz{0},
	max_sz{v.max_sz},
	values {new T [v.max_sz]}
	{
		for(; this->sz<v.sz; sz = sz + 1)
		{
			this->values[sz] = v.values[sz];
		}
	}

	//dritter konstruktor
	//konstruktor mit parameter: erzeugt einen vektor mit platz fuer n elemente
	//Vector third_vector (5);
	Vector(size_t n):
	sz{0},
	max_sz{n},
	values {new T [n]}
	{
		
	}
	
	//vierter konstruktor
	//konstruktor mit parameterliste: erzeugt einen vektor mit dem inhalt welcher durchgegeben wird
	//Vector fourth_vector(5,5.1,35,6365,8.01,5.6);
	Vector(std::initializer_list<T> list):
	sz{0},
	max_sz{list.size()},
	values {new T [list.size()]}
	{
		for(const auto& l : list)
		{
			values[sz] = l;
			sz = sz + 1;
		}
	}
	
	//dekonstruktor
	~Vector()
	{
		delete [] values;
	}
	
	
	//MEMBERFUNKTIONEN UND SO
	
	//kopierzuweisungsoperator =
	//das this objekt uebernimmt eine kopie des inhaltes des parameters
	//also das was links steht bekommt die eigenschaften/den inhalt welcher auf der rechten seite steht
	//first_vector = second_vector
	//hier bekommt first_vector den inhalt welcher in second_vector steht
	Vector operator = (const Vector& v)
	{
		this->sz = 0;
		//this->max_sz = v.max_sz;																	//die beiden hier muessen umgedreht werden weil wenn ich die this->max_sz auf v.max_sz setze, dann funktioniert reserver im utest nicht richtig.
		//this->reserve(v.max_sz); //NOCH NICHT IMPLEMENTIERT, KOMMT ABER NOCH		//bei meinen eigenen versuchen hat es richtig funktioniert, aber im utest nicht. keine ahnung wieso
		this->reserve(v.max_sz); //NOCH NICHT IMPLEMENTIERT, KOMMT ABER NOCH
		this->max_sz = v.max_sz;
		for(;this->sz<v.sz; sz = sz+1)
		{
			this->values[this->sz] = v.values[this->sz];
		}
		return *this;	//ka wieso man hier *this returnen muss. ich meine mich zu erinnern das man immer this retunrne muss bei operator = overload aber ka
	}
	
	//returnt die anzahl der aktuell gespeicherten elemente, also sz
	size_t size() const
	{
		return this->sz;
	}
	
	//returnt trie falls der vector leer ist, also falls sz = 0
	bool empty() const
	{
		if(this->sz == 0)
		{
			return true;
		}
		return false;
	}
	
	//loescht alle elemente aus dem vector
	//die elemente werden nicht wirklich geloscht, sondern sz wird einfach auf 0 zurreuckgesetzt weil man die alten elemente einfach ueberschreiben kann
	void clear()
	{
		this->sz = 0;
	}
	
	//reserviert n platz fuer den vektor falls der vektor nicht schon n gross ist (also falls max_sz nicht schon n gross ist)
	void reserve(size_t n)
	{
		if(this->max_sz < n)
		{
			T* temp {new T [this->sz]}; //dashier ist ein temporaeres array
			for(size_t i {0}; i < this->sz; i++) //hier wird ins temporere array der inhalt vom nicht temporeren array reingeschoben
			{
				temp[i] = this->values[i];
			}
			
			delete []this->values;	//der inhalt vom array auf welches der pointer values zeigt wird geloescht (damit) 
			values = new T [n]; //values auf einen neuen array an einer neuen speicherposition zeigt welches so viel speicher reserviert hat, wie n gross ist
			max_sz = n;					//max_sz ist jetzt so gross wie der parameter
			//sz = 0;
			
			for(size_t i{0}; i < sz; i = i + 1)
			{
				values[i] = temp[i];	//das neue array bekommt den inhalt vom alten (,geloeschten) array (zurreuck)
			}
			delete[] temp;				//temp wird geloescht weil es nicht mehr gebraucht wird. ka ob man das machen muss oder nicht weil es eh am ende der funktion ist aber ich mache es trotzdem mal
			//std::cout << "diese nachricht ist von reserve geschrieben und sz ist jetzt: " << this->size() << std::endl; 	//DEBUG
		}
	}
	
	//kapazitaet des vektors wird auf das minimum gesetzt
	//max_sz wird einfach so gross gemacht wie sz
	void shrink_to_fit()
	{
		max_sz = sz;
	}
	
	//fuegt x am ende des arrays hinzu
	void push_back(T x)
	{
		//std::cout << "hier ist die momentane sz: " << this->size() << std::endl;			//DEBUG
		//std::cout << "hier ist die momentane max_sz: " << this->capacity() << std::endl;	//DEBUG
		if(sz>=max_sz) //falls x gleich gross ist wie max_sz und man ein weitern eintrag hinzufuegen wuerde, dann waere sz groesser als max_sz und das geht nicht
		{					//weil es dann den naechsten eintrag im memory ueberschreiben wuerde aber der naechste eintrag gehoert nicht mehr zum array
			this->reserve((max_sz+1)*2);	//deshalb wird mehr speicher reserviert.
		}
		values[sz] = x;
		sz = sz+1;
	}
	
	//entfernt das letzte element im vector und falls der vector leer war, wirf runtime error
	//sz wird einfach um 1 kleiner gemacht weil es dann ueberschrieben werden kann mit neuen daten
	void pop_back()
	{
		if(this->empty())
		{
			throw std::runtime_error("Array war leer, du kannst nicht pop_back verwenden");
		}
		sz = sz - 1;
	}
	
	//returnt eine referenz auf das element an der position "index". falls "index" im nicht erlaubten bereich ist (index>sz) wirf runtime_error
	T& operator[](size_t index)
	{
		if(index>sz - 1)
		{
			throw std::runtime_error("Index ist im out of bounds (wahrscheinlich zu hoch als zu niedrig)");
		}
		return values[index];
	}
	
	//returnt eine referenz auf das element an der position "index". falls "index" im nicht erlaubten bereich ist (index>sz) wirf runtime_error
	//also basically das gleiche nur fuer const vectoren
	const T& operator[](size_t index) const
	{
		if(index>sz - 1)
		{
			throw std::runtime_error("Index ist im out of bounds (wahrscheinlich zu hoch als zu niedrig)");
		}
		return values[index];
	}
	
	//returnt die kapazitaet des vectors, also max_sz
	size_t capacity() const
	{
		return this->max_sz;
	}
	
	//print funktion
	std::ostream& print(std::ostream& o) const
	{
		o << "[";
		bool first {true};
		for(size_t i {0}; i < sz; i = i + 1)
		{
			if(first == true)
			{
				first = false;
			}
			else
			{
				o << ", ";
			}
			o << values[i];
		}
		o << "]";
		return o;
	}

	//operator << overload mit friend funktion
	//FOOTNOTE 1
	friend std::ostream& operator << (std::ostream& o, const Vector& v)
	{
		return v.print(o);
	}
	
	//returnt einen iterator der auf das erste element im vector zeigt, bzw gleich dem end-iterator ist falls der vector leer ist
	iterator begin()
	{
		return iterator {values, values+sz};
	}
	
	//returnt einen interator der auf das element hinter dem letzten element im vector zeigt
	iterator end()
	{
		return iterator {values+sz ,values+sz};
	}
	
	//das gleich nur nochmal mit const
	//returnt einen iterator der auf das erste element im vector zeigt, bzw gleich dem end-iterator ist falls der vector leer ist
	const_iterator begin() const
	{
		return const_iterator {values, values+sz};
	}
	
	//returnt einen interator der auf das element hinter dem letzten element im vector zeigt
	const_iterator end() const
	{
		return const_iterator {values+sz ,values+sz}; //ka ob hier {values, values+sz} oder nur {values+sz} hingehoert
	}
	
	
	
	//NICHT VON MIR. IN DER ANGABE STEHT ICH SOLL ES ABSCHREIBEN
		
	iterator insert(const_iterator pos, const_reference val)
	{
		auto diff = pos-begin();
		if(diff<0 || static_cast<size_type>(diff)>sz)
		{
			throw std::runtime_error("Iterator out of bounds");
		}
		size_type current{static_cast<size_type>(diff)};
		if(sz>=max_sz)
		{
			reserve(max_sz*2+1); //Attention special case, if no minimum size is defined
		}
		for(auto i {sz}; i-->current;)
		{
			values[i+1]=values[i];
		}
		values[current]=val;
		++sz;
		return iterator{values+current};
	}
	
	iterator erase(const_iterator pos)
	{
		auto diff = pos-begin();
		if(diff<0 || static_cast<size_type>(diff)>=sz)
		{
			throw std::runtime_error("Iterator out of bounds");
		}
		size_type current{static_cast<size_type>(diff)};
		for(auto i{current}; i<sz-1; ++i)
		{
			values [i]=values[i+1];
		}
		--sz;
		return iterator{values+current};
	}
	
	//NICHT VON MIR. IN DER ANGABE STEHT ICH SOLL ES ABSCHREIBEN
	
	
	//Iterator klasse
	class Iterator
	{
	public:
		
		//aliases und so
		using value_type = Vector::value_type;
		using reference = Vector::reference;
		using pointer = Vector::pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
	
	private:
		//instanz variablen
		pointer ptr; //zeigt auf das entsprechende element im vektor (man muss hier nicht pointer* ptr machen weil pointer bereits ein alias hat: pointer = Vector::pointer; pointer = value_type*;)
		pointer end; //zeigt auf das letzte element im vektor. steht zwar nicht in der angabe aber ist ist besser so fuer absicherung 
		
	public:
	
#if 0
		//konstruktoren
		
		//1 konstruktor: default konstruktor
		//erzeugt einen interator welcher auf nullptr zeigt
		//Iterator first_iterator();
		Iterator():
		ptr{nullptr},
		end{nullptr} //ka ob man dashier braucht
		{
		
		}
		

		//2 konstruktor: parameterliste
		//erzeugt einen iterator anhand den parametern
		//Iterator second_iterator()
		Iterator(pointer ptr):
		ptr{ptr}
		{

		}

		
		//3 konstruktor: parameterliste
		//erzeugt einen iterator anhand den parametern
		//Iterator second_iterator()
		Iterator(pointer ptr, pointer end):
		ptr{ptr},
		end{end}
		{
			
		}
#endif
		Iterator(pointer ptr = nullptr, pointer end = nullptr):
		ptr{ptr},
		end{end}
		{

		}


		//member funktionen
		
		//returnt eine referenz auf den wert auf den der iterator zeigt (=auf den die instanzvariable ptr zeigt)
		reference operator*() const //kein const
		{
			if(ptr == end)
			{
				throw std::runtime_error("Pointer is out of bounds. thrown from: class Iterator. method: reference operator*");
			}
			return *ptr;
		}
		
		//returnt einen pointer auf den von iterator referenzierten wert
		pointer operator -> () const //kein const
		{
			if(ptr == end)
			{
				throw std::runtime_error("Pointer is out of bounds. thrown from: class Iterator. method: pointer operator ->*");
			}
			return ptr;
		}
		
		//operator == overload
		bool operator == (const const_iterator& ci) const
		{
			if(static_cast<const_iterator>(this->ptr) == ci) //(static_cast<const_iterator>(ci) == this->ptr) funktioniert auch aus irgendeinem grund, weiss aber nicht wieso
			{
				return true;
			}
			return false;
		}
		
		//operator != overload
		bool operator != (const const_iterator& ci) const
		{
			if(static_cast<const_iterator>(this->ptr) == ci) //(static_cast<const_iterator>(ci) == this->ptr) funktioniert auch aus irgendeinem grund, weiss aber nicht wieso
			{
				return false;
			}
			return true;
		}
		
		//iterator wird auf das naechste element im vector weitergescahltet
		//prefix
		iterator& operator ++ ()
		{
			bool ptr_is_end{false};
			if(ptr == end)
			{
				ptr_is_end = true;
			}
			if(ptr_is_end == true)
			{
				return *this;
			}
			ptr = ptr + 1;
			return *this;
		}
		
		//iterator wird auf das naechste element im vector weitergescahltet
		//postfix
		iterator operator++(int)
		{
			bool ptr_is_end{false};
			if(ptr == end)
			{
				ptr_is_end = true;
			}
			if(ptr_is_end == true)
			{
				return ptr;
			}
			pointer temp {ptr};
			ptr = ptr + 1;
			return temp;
		}
		
		operator const_iterator() const
		{
			return ConstIterator{ptr, end};
		}
		
	
	};
	
	//ConstIterator klasse
	class ConstIterator
	{
	public:
	
		//aliases und so
		using value_type = Vector::value_type;
		using reference = Vector::const_reference;
		using pointer = Vector::const_pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;

	private:
		//instanz variablen
		pointer ptr; //zeigt auf das entsprechende element im vektor (man muss hier nicht pointer* ptr machen weil pointer bereits ein alias hat: pointer = Vector::pointer; pointer = value_type*;)
		pointer end; //zeigt auf das letzte element im vektor. steht zwar nicht in der angabe aber ist ist besser so fuer absicherung 
		
	public:

		//konstruktoren
#if 0
		//1 konstruktor: default konstruktor
		//erzeugt einen interator welcher auf nullptr zeigt
		//Iterator first_iterator();
		ConstIterator():
		ptr{nullptr},
		end{nullptr} //ka ob man dashier braucht
		{
		
		}
		
		//2 konstruktor: parameterliste
		//erzeugt einen iterator anhand den parametern
		//Iterator second_iterator()
		ConstIterator(pointer ptr):
		ptr{ptr}
		{

		}
		
		//3 konstruktor: parameterliste
		//erzeugt einen iterator anhand den parametern
		//Iterator second_iterator()
		ConstIterator(pointer ptr, pointer end):
		ptr{ptr},
		end{end}
		{
			
		}
#endif
		ConstIterator(pointer ptr = nullptr, pointer end = nullptr):
		ptr{ptr},
		end{end}
		{

		}


		//methoden
	
		//returnt eine referenz auf den wert auf den der iterator zeigt (=auf den die instanzvariable ptr zeigt)
		reference operator*() const
		{
			if(ptr == end)
			{
				throw std::runtime_error("Pointer is out of bounds. thrown from: class ConstIterator. method: reference operator*() const");
			}
			return *ptr;
		}
		
		//returnt einen pointer auf den von iterator referenzierten wert
		pointer operator -> () const
		{
			if(ptr == end)
			{
				throw std::runtime_error("Pointer is out of bounds. thrown from: class ConstIterator. method: pointer operator ->* const");
			}
			return ptr;
		}
		
		//operator == overload
		bool operator == (const const_iterator& ci) const
		{
			if(ci.ptr == this->ptr)
			{
				return true;
			}
			return false;
		}
		
		//operator != overload
		bool operator != (const const_iterator& ci) const
		{
			if(ci.ptr == this->ptr)
			{
				return false;
			}
			return true;
		}
		
		//iterator wird auf das naechste element im vector weitergescahltet
		//prefix
		const_iterator& operator ++ ()
		{
			bool ptr_is_end {false};
			if(ptr == end)
			{
				ptr_is_end = true;
			}
			if(ptr_is_end == true)
			{
				return *this;
			}
			ptr = ptr + 1;
			return *this;
		}
		
		//iterator wird auf das naechste element im vector weitergescahltet
		//postfix
		const_iterator operator ++ (int)
		{
			bool ptr_is_end{false};
			if(ptr == end)
			{
				ptr_is_end = true;
			}
			if(ptr_is_end == true)
			{
				return ptr;
			}
			pointer temp {ptr};
			ptr = ptr + 1;
			return temp;
		}
	
	
		//NICHT VON MIR. IN DER ANGABE STEHT ICH SOLL ES ABSCHREIBEN
		friend Vector::difference_type operator-(const Vector:: ConstIterator& lop, const Vector::ConstIterator& rop)
		{
			return lop.ptr-rop.ptr;
		}
		//NICHT VON MIR. IN DER ANGABE STEHT ICH SOLL ES ABSCHREIBEN
	
	};
	
};

/*
//FUNKTIONIERT SET CHAPTER 3 TEMPLATE NICHT MEHR. ICH MACHE STATTDESSEN EINE FRIEND METHODE. FOOTNOTE 1
//operator << overload
std::ostream& operator << (std::ostream& o, const Vector& v)
{
	return v.print(o);
}
*/




















#endif


