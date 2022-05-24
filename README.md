﻿### 2UTM - до 10ти УТМ на одной машине


Обновление 1.1 - релиз можно скачать тут: https://github.com/2UTM/2UTM/releases  
По ошибкам и вопросам просьба писать тут:  
GitHub - https://github.com/2UTM/2UTM/issues  
Почта - 2UTM.dev@gmail.com  
Telegrem - @regnorr  


Изменения в 1.1  
1. Изменен внешний вид интерфейса, приближен к интерфейсу драйвера рутокен  
2. Изменены все алгоритмы, устранены ошибки с дочерними именами, убраны лишние зависимости, задействовано API смарт карт  
3. Добавлена возможность установки до 10ти УТМ (так как может быть только 10 ридеров для смарт карт)  
4. Добавлена возможность изменения порта для УТМ  
5. Убрана зависимость от порядка токенов в системе  




Небольшое введение, кто не в курсе что такое УТМ и с чем его едят и почему появилась необходимость в 2UTM...

Все наверное слышали про систему ЕГАИС, через которую торгуют алкоголем (только в России, насколько я знаю). УТМ - это универсальный траспортный модуль, через который чеки отправляются в ФСРАР, так же через него происходит весь документооборот между поставищком и розничными продавцами алкгольной продукции. УТМ разработан ФСРАР, и скачать его можно в личном кабинете ЕГАИС, либо на каком нибудь другом ресурсе, посвященному работе в нем. Так же ФСРАР утверждают что два УТМ на одну машину установить невозможно...



О программе 2UTM...

Где то месяца 2 назад появилась необходимость установить два УТМ на одну машину, так как есть две базы 1С и они должны функционировать одновременно. Сразу появилась идея, так как ФСРАР утверждает что 2 УТМ быть не может, сделать виртульную машину с пробросом портов. Сначала так и сделали, но на практике оказалось очень неудобна и расточительна такая схема работы, и было принято решение исследовать данную проблему и, по возможности, найти программное решение. Так же, так как это писалось для продавцов, нужно было сделать максимально простую и автоматизированную программу, что бы поняли даже бабушки с 3-мя классами церковно приходской. Аналоги я находил, но они топорны и работают через раз.
Соответсвенно была реализована программа, так как она написана на Qt комьюнити, я не имею права продавать ее и закрывать код, поэтому она и здесь... 



Функционал 2UTM...

Для программы нужны административные права, так как присутствует работа с оборудованием и установкой стороних программ (УТМ). Она умеет отключать рутокены (носители цифровой подписи), устанавливать/удалять УТМы, запускать оба УТМа, есть возможность в настройках указать, как ее запускать (свернутой в трей или нет, при запуске программы запускать УТМы или нет и закрывать программу после успешного запуска УТМов).
Программа работает только с рутокенами, так как для джакарт нельзя так просто скачать sdk (почему, для меня загадка), нужно оставлять заявку и может быть вам его вышлют, и еще задачи не стояло, чтобы джакарты тоже поддерживались. Если у вас есть необходимьсть реализовать тот же функционал с джакартами, можете написать здесь на гитхабе или на почту 2UTM.dev@gmail.com. Открыть проект можно через QtCreator...



Пункты меню...

Файл -> Выход - выход


Операции -> Прочитать устройства - получить информацию об устройствах (токенах)

Операции -> Включить устройство - включает выделенное устройство

Операции -> Отключить устройство - отключает выделенное устройство

Операции -> Переподключить устройства - заметил, что на некоторых машинах рутокены не правильно определяются в диспетчере устройств, из за этого я не могу получить их дочерние элементы, решаеться переподключением устройств физически или программно

Операции -> Запустить УТМ - запускает оба УТМа

Операции -> Остановить УТМ - останавливает оба УТМа

Операции -> Поменять порядок устройств - меняет порядок ключей (рутокенов) в конфиге, УТМ привязан к определному ключу и на другом не запуститься, если вытащить и вставить рутокены в другом порядке, нужно выполнить эту операцию

Операции -> Настройки - настройки


Установка -> Установить утм - устанавливает оба УТМ и запоминает порядок рутокенов

Установка -> Удалить утм - удаляет оба УТМ


Справка -> О программе - о программе

Справка -> Связь с разработчиком - связь со мной, ссылка на гитхаб и почта


Спасибо за внимание!

PS
Если есть желание отблагодарить материально - можете написать на почту


Ссылка на гитхаб - https://github.com/2UTM/2UTM

Почта - 2UTM.dev@gmail.com
