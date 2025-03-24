# Класове
Класът в С++ е изключително подобен на структурата. Както структурите, класовете се използват за да опишем обект. Единствената разлика между двете е *видимостта* на данните.

## Видимост на данни
При структурите, след като напишем дефиницията, можем да създаваме обекти и да използваме техните полета свободно:

```cpp
struct Point
{
    int x, y;
};

Point point;
point.x = 0;
point.y = 2;
```

Ако пробваме да направим същото нещо с клас, няма да успеем да компилираме кода, защото данните в класовете са скрити по подразбиране:
```cpp
class Point
{
    int x, y;
};

Point point;
point.x = 0; // error
point.y = 2; // error
```
---

За да използваме коректно класовете, трябва да се запознаем с първите 2 принципа на ООП - **Капсулация** и **Абстракция**

## Капсулация (Encapsulation)
Капсулацията представлява скриване на данни, които не искаме да могат да се променят от външния свят. Това се постига с *модификаторите* `public` и `private`
- **public**: позволява достъп до полето извън класа
- **private**: забранява достъп до полето извън класа

В структурите, освен ако не окажем експлицитно, по подразбиране всички полета са `public`, за разлика от класовете, където всички полета по подразбиране са `private`.  

```cpp
class Point
{
public:
    int x;
    int y;

private:
    int z;
};

Point point;
point.x = 0; // OK
point.y = 2; // OK
point.z = 10; // error
```

## Абстракция (Abstraction)
Абстракцията представлява скриване на имплементацията с помощта на *методи*. Това позволява за по-сложна логика, скрита от външния свят. Често това се случва с т.нар. селектори и мутатори (getters & setters):

```cpp
class Circle
{
private:
    float radius;

public:
    void setRadius(float newRadius)
    {
        if (newRadius <= 0)
            return; // the radius cannot be <= 0

        radius = newRadius;
    }

    float getRadius()
    {
        return radius;
    }

    float calculateArea()
    {
        return 3.14f * radius * radius;
    }
};

Circle circle;

circle.setRadius(2);
circle.setRadius(-2); // still 2
std::cout << circle.calculateArea();
```

---

Правилното използване на тези 2 принципа е ключово за да пишем добър ООП код.

--- 

## Конструктор
За да избегнем това викане на методи след като сме създали обект, има специализиран метод, т.нар. *конструктор*, който е метод, който се извиква при създаване на обект. Сега ще разгледаме 2 вида конструктори, въпреки че има още няколко.

### Конструктор по подразбиране (Default constructor)
Това е конструкторът, който се вика при просто създаване на обект.
```cpp
class Circle
{
private:
    float radius;

public:
    Circle()
    {
        setRadius(1);
    }

    // ... void setRadius(float)
};

Circle circle1; // radius == 1
Circle circle2(); // error: това е декларация на функция
```

### Конструктор с параметри (Parametrized constructor)
Това е конструктор, който се вика при създаване на обект с подаване на параметри

```cpp
class Circle
{
private:
    float radius;

public:
    Circle(float newRadius)
    {
        setRadius(newRadius);
    }

    // ... void setRadius(float)
};

Circle circle1; // error: няма default constructor
Circle circle2(2); // radius == 2
```

---

Позволено е да имаме няколко параметризирани конструктори, но само един дефолтен.

---


## Member initializer list
Когато няма нужда от валидация на данни, можем да използваме инициализиращия списък (member initializer list) за да зададем стойностите директно на членовете. Това ускорява процеса на създаване на обект, защото по подразбиране всички членове се инициализират с default стойност/конструктор, а с инициализиращия списък можем да пропуснем тази стъпка.

```cpp
class ComplexNumber
{
private:
    float real;
    float imaginary;

public:
    ComplexNumber(float r, float i)
        : real(r), imaginary(i)
    {
    }
};
```

## const
По-рано обсъдихме как е добра практика да подаваме обекти по `const&`, когато са по-обемни. Нека например имаме клас Person, който има име, възраст и височина. Една имплементация може да изглежда по следния начин:

```cpp
class Person
{
private:
    char name[32];
    short age;
    float height;

public:
    Person(const char* personName, short personAge, float personHeight)
    {
        setName(personName);
        setAge(personAge);
        setHeight(personHeight);
    }

    void setName(const char* newName)
    {
        if (newName == nullptr)
            return;

        strcpy(name, newName);
    }

    void setAge(short newAge)
    {
        if (newAge <= 0)
            return;

        age = newAge;
    }

    void setHeight(float newHeight)
    {
        if (newHeight <= 0)
            return;

        height = newHeight;
    }

    char* getName()
    {
        return name;
    }

    short getAge()
    {
        return age;
    }

    float getHeight()
    {
        return height;
    }
};
```

До тук добре*. Този обект е сравнително обемен, за това бихме го подали като `const&` на някоя функция, например за принтиране. 

```cpp
void printPerson(const Person& person)
{
    std::cout << person.getName() << ' '
              << person.getAge() << ' '
              << person.getHeight() << '\n';
}
```

Получаваме грешка. Нямаме право на достъп то тези методи, въпреки че са `public`. Причината за това е в параметъра - подали сме го като `const`, съответно С++ иска гаранция, че тези методи няма да променят обекта по някакъв начин. Тази гаранция се дава отново, с думата `const`, само че се слага след декларацията на метода.

```cpp
class Person
{
private:
    // ...
public:
    //...

    char* getName() const
    {
        return name;
    }

    short getAge() const
    {
        return age;
    }

    float getHeight() const
    {
        return height;
    }
};
```

Сега вече езикът няма да ни позволи да променим каквото и да е по класа от вътре в самите методи.

---

Почти сме готови, само чеее:

```cpp
void printPerson(const Person& person)
{
    person.getName()[0] = 'O'; // OK, but shouldn't be

    std::cout << person.getName() << ' '
              << person.getAge() << ' '
              << person.getHeight() << '\n';
}
```

Позволено ни е да модифицираме името, въпреки че методът е деклариран като `const`. Това се случва защото ние модифицираме името от *извън* метода. Дори обекта да е `const`, ако не внимаваме с тези неща, можем да позволим на външния свят да променя неща по него. За да забраним промяната на името от външния свят, просто трябва да връщаме `const char*`

```cpp
class Person
{
private:
    // ...
public:
    //...

    const char* getName() const
    {
        return name;
    }

    //...
};
```


## Указател към структура/клас
Когато правим указатели към структури и/или класове, в С++ има особеност, че вместо оператор `.` се използва оператор `->` когато се опитваме да достъпим полета или методи:

```cpp
struct Point
{
    int x, y;
};

Point point {10, 20};
Point* p = &point;

std::cout << p->x << " " << p->y;
```

## this
В класовете и структурите, **this** е *указател* към текущата инстанция. Това означава "указател към себе си". Този указател игнорира модификаторите public/private, за него всичко е видимо. Използва се най-често когато има препокриване (shadowing) на име на поле и аргумент:

```cpp
class Circle
{
private:
    float radius;

public:
    Circle(float radius) // shadowing the field 'radius'
    {
        this->radius = radius;
    }
};
```

## На практика
Въпреки че единствената разлика между класовете и структурите е видимостта на данните, на пракика двете се използват за различни цели. Структурите често се използват за по-прости конструкции, напр. Point, Vector, Color и т.н.. Класовете се използват за по-сложни конструкции, напр. Student и Person.  

Ето един cheat sheet за това кое кога се използва:

Тип | struct | class
--- | --- | ---
Проста конструкция | ✅ | ❌
Нужна капсулация | ❌ | ✅
Нужна логика и поведение | ❌ | ✅
Нужен конструктор | ✅(рядко) | ✅
Сложен жизнен цикъл | ❌ | ✅