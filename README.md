# **DataBase-server**

Код для сервера баз данных. Работает черед Vinsocket порт 3487
Воспринимает команды: 
1. Создание новой таблицы
'<C <имя таблицы> <количество полей>>'

где <имя таблицы> - любая последовательность английских букв и цифр (заглавная\прописная значения не имеет)
<количество полей> - целое число, определяющее кол-во вещественных (float) параметров, сохраняемых в таблицу
2. Удаление таблиц
'D <имя таблицы>'
3. Добавление записи в таблицу
'I <имя таблицы> (<параметр>[, <параметр>])'
где <параметр> - вещественное число. Синтаксис не предусматривает ввода чисел в формате со степенью: ХеХ
#**Авторы**
Денисова Елена, Ожегов Андрей, Норицына Светлана
