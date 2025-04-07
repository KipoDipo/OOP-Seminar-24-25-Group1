# Жизнен цикъл
Вече знаем, че животът на една променлива започва от нейната инициализация до края на scope-a, в който е инициализирана. При класовете и структурите това правило все още важи. При тях жизнения цикъл започва успешното приключване на конструктора, до края на scope-a. Нека погледнем следния код:

```cpp
class Person
{
    char name[32];
    int age;

public:
    Person(const char* name, int age)
    {
        strcpy(this->name, name);
        this->age = age;
    }
};
```

```cpp
{
    Person ivan("Ivan", 20); // ivan is created
} // ivan is destroyed
```

1) Създават се char масив и int на стека.
2) Запълваме съдържанието на char масива и int-а приема стойност. 
3) Унищожават се масива и int-a

Нека обаче променим нашия клас така, че името винаги да заема точно толкова памет, колкото трябва.

```cpp
class Person
{
    char* name;
    int age;

public:
    Person(const char* name, int age)
    {
        this->name = new char[strlen(name) + 1] {};
        strcpy(this->name, name);
        this->age = age;
    }
};
```

```cpp
{
    Person ivan("Ivan", 20); // ivan is created
} // ivan is destroyed
```

1) Създава се указател и int на стека.
2) Указателя започва да сочи към динамично заделена памет и int-а приема стойност. 
3) Унищожават се указателя и int-a
4) **Динамичната памет остава заделена** -> ***Memory leak***

## Деструктор
Тук идва идеята на деструктора - специална функция, която се вика сама в края на жизнения цикъл на обекта. Деструктора няма параметри и никога* не се вика експлицитно.

```cpp
class Person
{
    char* name;
    int age;

public:
    Person(const char* name, int age)
    {
        this->name = new char[strlen(name) + 1] {};
        strcpy(this->name, name);
        this->age = age;
    }
    ~Person()
    {
        delete[] name;
    }
};
```

```cpp
{
    Person ivan("Ivan", 20); // ivan is created
} // ivan is destroyed
```

1) Създават се указател и int на стека.
2) Указателя започва да сочи към динамично заделена памет и int-а приема стойност. 
3) Извиква се деструктора *~Person()* -> динамичната памет се освобождава
4) Унищожават се указателя и int-a

## Двоични файлове при класове с указатели като член данни
Когато записваме двоичен файл по стандартния начин:

```cpp
Data data {1, 'g'};

ofstream out("file.bin", ios::binary);

out.write(reinterpret_cast<const char*>(&data), sizeof(data));
```
във файла се записват всички полета едно след друго (вкл. padding).

---

Когато, обаче, се опитаме да приложим същата техника при файлове с указатели, напр. указател към динамична памет:

```cpp
Person ivan("Ivan", 20);

ofstream out("file.bin", ios::binary);

out.write(reinterpret_cast<const char*>(&ivan), sizeof(ivan));
```

всъщност ще запишем не съдържанието на динамичния масив, а на указателя, т.е. *адреса* на динамичния масив за името. В такива случаи не можем да запишем в двоичен файл на един ред, както до сега. Трябва класът да има getter-и и setter-и, с които да можем да запишем и прочетем всички полета, едно по едно; когато се налага да записваме динамични масиви, често записваме размерът им преди самото съдържание:

```cpp
Person ivan("Ivan", 20);

ofstream out("file.bin", ios::binary);

int nameLength = strlen(ivan.getName());
int age = ivan.getAge();

out.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
out.write(reinterpret_cast<const char*>(ivan.getName()), nameLength);
out.write(reinterpret_cast<const char*>(&age), sizeof(age));
```

*Notes:*
1) Записваме дължината на масива, нещо което не е част от оригиналния клас, но нужно за да можем да прочетем коректно масива при четене от файла
2) `ivan.getName()` връща указател, съответно не е нужно да се взима адреса (`&`)
3) **не** можем да напишем `reinterpret_cast<const char*>(&ivan.getAge())`, защото `getAge()` връща **копие** на полето `age`, а временните стойности в С++ нямат адреси (както например не можем да напишем `&4`, `&false`, `&(a + b)`). За това записваме getAge() в променлива, за да можем да ѝ вземем адреса при записване във файл.

```cpp
ifstream in("file.bin", ios::binary);

int nameLength;
char* name;
int age;

in.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

name = new char[nameLength + 1] {};

in.read(reinterpret_cast<char*>(name), nameLength);

in.read(reinterpret_cast<char*>(&age), sizeof(age));

Person ivan(name, age);

delete[] name;
```

## Конструктор за присвояване (Copy constructor)

До тук копирането на данни е било тривиално. Например:

```cpp
struct Point
{
    int x, y;
};

Point А {1, 2};
Point B = А;
```
`А` и `B` са 2 два отделни обекта в паметта, управляващи собствените си данни (координатите `x` и `y`). Това обаче може да създаде проблем, когато копираме данни като указатели.

```cpp
class Person
{
    char* name;
    int age;

public:
    Person(const char* name, int age)
    {
        this->name = new char[strlen(name) + 1] {};
        strcpy(this->name, name);
        this->age = age;
    }
    ~Person()
    {
        delete[] name;
    }
};
```

```cpp
Person originalDavid("David", 30);
Person anotherDavid = originalDavid;
```

Сега и двата обекта съдържат указател, който сочи към една и съща памет. На пръв поглед това може да не изглежда като особено голям проблем, стига да не променяме съдържанието на низа, но истинския проблем идва при приключването на живота на двата обекта.

Викайки деструктора на `originalDavid`, паметта, посочена от `name`, се освобождава. Проблемът идва при повикване на деструктора на `anotherDavid`, където се опитваме да освободим същата памет, която вече е освободена. Последствието е crash на програмата, като този случай често трудно се проследява (**double free**). Решението е т.нар. ***Copy Constructor***, който е създаден с цел да контролираме как точно се копира един обект.

```cpp
class Person
{
    char* name;
    int age;

public:
    Person(const char* name, int age)
    {
        this->name = new char[strlen(name) + 1] {};
        strcpy(this->name, name);
        this->age = age;
    }
    Person(const Person& other)
    {
        this->name = new char[strlen(other.name) + 1] {};
        strcpy(this->name, other.name);
        this->age = other.age;
    }
    ~Person()
    {
        delete[] name;
    }
};
```

Copy Constructor-ът приема само 1 параметър - константен псевдоним от тип класът. Сега вече когато направим 2 обекта и копираме единия в другия, двата ще сочат към собствени заделени памети, и двата ще се грижат за собствено задалената си памет. 

***Важно:*** Всеки клас трябва да се грижи за собствената си памет

```cpp
Person originalDavid("David", 30);
Person anotherDavid = originalDavid;
//или
Person anotherDavid(originalDavid);
```

## Оператор за присвояване (Copy assignment operator)

Ако копирането на един обект в друг се случи в по-късен етап от жизнения му цикъл, тогава се вика т.нар. *Оператор за присвояване* (или *operator =*). Припомняме си - конструктора на един обект се вика само и единствено при неговото създаване, и никога след това. 

```cpp
Person originalDavid("David", 30);
Person impostor("Impostor", 99);

impostor = originalDavid;
// same problem
```

С++ ни позволява да си пишем собствени оператори (+, -, \*, /, =, ==, !=, ! и т.н.).

Да се пробваме да си напишем наш *operator =*, като ще го подобряваме през няколко итерации

```cpp
class Person
{
    char* name;
    int age;

public:
    // ctor, copy ctor
    
    void operator=(const Person& other)
    {
        this->name = new char[strlen(other.name) + 1] {};
        strcpy(this->name, other.name);
        this->age = other.name
    }

    // dtor
};
```

Добре, само че всеки път като извикаме operator= ще получаваме memory leak, защото не сме изчистили старата памет.

```cpp
Person originalDavid("David", 30); // ok
Person originalPetko("Petko", 29); // ok

Person impostor("Impostor", 99); // ok

impostor = originalDavid; // memory leak
impostor = originalPetko; // memory leak
```

```cpp
class Person
{
    char* name;
    int age;

public:
    // ctor, copy ctor
    
    void operator=(const Person& other)
    {
        delete[] this->name;
        this->name = new char[strlen(other.name) + 1] {};
        strcpy(this->name, other.name);
        this->age = other.name
    }

    // dtor
};
```

Добре, обаче ако приравним обект към себе си, ще изтрием паметта, от която след това се очаква да четем и ще crash-нем приложението.

```cpp
Person originalDavid("David", 30); // ok

originalDavid = originalDavid; // crash
```

```cpp
class Person
{
    char* name;
    int age;

public:
    // ctor, copy ctor
    
    void operator=(const Person& other)
    {
        if (this != &other)
        {
            delete[] this->name;
            this->name = new char[strlen(other.name) + 1] {};
            strcpy(this->name, other.name);
            this->age = other.name
        }
    }

    // dtor
};
```

Добре, сега искаме да можем прилагаме верижно operator=, както можем с примитивните типове.

```cpp
int x, y, z;
x = y = z = 0;

Person defaultGuy1("", 0), defaultGuy2("", 0);
Person defactoGuy("De Facto", 32);

defaultGuy1 = defaultGuy2 = defactoGuy; // error
```

```cpp
class Person
{
    char* name;
    int age;

public:
    // ctor, copy ctor
    
    Person& operator=(const Person& other)
    {
        if (this != &other)
        {
            delete[] this->name;
            this->name = new char[strlen(other.name) + 1] {};
            strcpy(this->name, other.name);
            this->age = other.name
        }

        return *this;
    }

    // dtor
};
```

До това се свежда най-типичната (с [Basic Exception Guarantee](https://en.cppreference.com/w/cpp/language/exceptions)) имплементация на operator=.

## Изключения (exceptions)
Изключенията прекъсват изпълнението на програмата докато не бъдат "хванати". Ако няма код, който да хване това прекъсване, изпълнението на програмата приключва изцяло. Изключенията могат да представляват на практика всичко - int, char, Person и т.н.. 

```cpp
float div(float dividend, float divisor)
{
    if (divisor == 0)
        throw "Can't devide by 0";

    return dividend / divisor;
}
```

Ако сега извикаме тази функция, и не вземем мерки за изключнието, програмата ще прекъсне своето изпълнение.

```cpp
int main()
{
    float result = div(10, 0);

    std::cout << "Hello, world!";
}
```

Изход:
```
```

За да хванем и обработим грешка, използваме т.нар. `try/catch` блок. В `try` слагаме израза, който очакваме да хвърли изключението, а в `catch` указваме какъв е типът на данни, които очакваме да получим в резултат на изключението (в този случай `const char*`), след което обработваме грешката. 


```cpp
int main()
{
    try
    {
        float result = div(10, 0);
    }
    catch (const char* message)
    {
        std::cout << "An error occured: " << message << '\n';
    }

    std::cout << "Hello, world!";
}
```

Изход:
```
An error occured: Can't devide by 0
Hello, world!
```

Ако не знаем какъв е типът, или искаме просто да хванем каква да е грешка, можем да използваме и следния синтаксис:
```cpp
int main()
{
    try
    {
        float result = div(10, 0);
    }
    catch (int code)
    {
        std::cout << "An error occured with code: " << code << '\n';
    }
    catch (const char* message)
    {
        std::cout << "An error occured: " << message << '\n';
    }
    catch (...)
    {
        std::cout << "An error occured!\n";
    }

    std::cout << "Hello, world!";
}
```

Можем да имаме колкото искаме `catch` блокове, като ако използваме `(...)`, той трябва да е най-накрая, иначе ще бъде прихванат първи.

```cpp
int main()
{
    try
    {
        float result = div(10, 0);
    }
    catch (...)
    {
        std::cout << "An error occured!\n";
    }

    std::cout << "Hello, world!";
}
```

### При класовете
Ако се хвърли изключение, по време на извикването на конструктор, обектът не се създава, следователно и неговия деструктор не се извиква.

```cpp
class Data
{
public:
    Data(int size1, int size2)
    {
        data1 = new int[size1];
        data2 = new int[size2]; 
        // if data2 throws, data1 will cause a memory leak, since ~Data() will not be called
    }
    ~Data()
    {
        delete[] data1;
        delete[] data2;
    }
//...
}
```

Във всички други случаи, ако се хвърли изключение, стековите обекти, създадени до този момент, ще бъдат унищожени, все едно scope-а е приключил там където е думата `throw`.

## Обобщение и още две приказки
- Прилагането на тези специални методи е задължителен процес, когато един клас борави с динамична памет. Това изискване се среща като *Голямата Четворка* (ctor, copy ctor, operator=, dtor), или като *Rule of 3* (copy ctor, operator=, dtor). 
    - *Rule of 3* означава, че ако ни трябва едно от тези 3 неща, вероятно ни трябват и другите 2
- Всеки клас **трябва** да отговаря за собствената си памет
- Copy Constructor-a (както всеки останал) се вика **само** при създаване на обекта. 

    ```cpp
    Person david("David");

    Person impostor = david;   // copy ctor
    Person impostor(david);    // copy ctor
    impostor = david;          // operator=
    ```
- Жизнения цикъл на стековите обекти приключва с приключването на scope-а в който се намират. При динамичните обекти, жизненият цикъл приключва, когато е извикан `delete`/`delete[]`
    - За да избегнете случайно изтриване на стекови обекти (което не е позволено), използвайте един указател или само за стекова или само за динамична памет
    ```cpp
    Person* ptr = nullptr;

    if (var % 2 == 0)
    {
        ptr = new Person("Petko", 28);
    }

    if (var % 3 == 0)
    {
        delete ptr; // calls ~Person()

                    // if Petko was assigned, now he is destroyed

                    // if Petko wasn't assigned,
                    // it's still absolutely OK to delete
                    // nullptr

                    // always delete a dynamic object before
                    // assigning a new value to the pointer

        ptr = new Person("David", 30);
    }

    delete ptr; // calls ~Person()

                // if either Petko or David was assigned
                // now they are destroyed
                
                // if none of them were ever assigned,
                // it's still absolutely OK to delete
                // nullptr
    ```

## Улеснение
За да избегнем писането на подобен код в copy ctor-a и operator=, можем да използваме 2 метода - `copy(const Person& other)` и `free()`. Тези методи трябва да са достъпни само и единствено от класа. Това не е задължително, но намалява обема написан код.

```cpp
class Person
{
public:

    Person(const char* name, int age)
    {
        this->name = new char[strlen(name) + 1] {};
        strcpy(this->name, name);
        this->age = age;
    }
    Person(const Person& other)
    {
        copy(other);
    }
    Person& operator=(const Person& other)
    {
        if (this != &other)
        {
            free();
            copy(other);
        }
        return *this;
    }
    ~Person()
    {
        free();
    }

private:
    void copy(const Person& other)
    {
        this->name = new char[strlen(other.name) + 1] {};
        strcpy(this->name, other.name);
        this->age = other.name
    }
    void free()
    {
        delete[] name;
    }

    char* name;
    int age;
};
```