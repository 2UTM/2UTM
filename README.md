### В связи с набором популярности программы, которой я не ожидал, хотелось бы прояснить некоторые моменты, в связи с которыми возникают частые вопросы...
Заметьте, что у программы нет никакой лицензии, то есть это, по сути, экспериментальное ПО, стабильность никто не гарантирует, если вам кто то сказал, что все работает стабильно, можете пнуть его ногой в живот.

Так же, если у вас ошибки с моей программой и вы хотите их исправить, можете предложить мотивацию, и я выделю время, чтобы решить вашу проблему.

Так же можете сами исправить проблемы, повторюсь, у ПО нет никакой лицензии, вы можете его использовать, изменять и внедрять где и куда угодно, но на свой страх и риск.

Разработчики утм тоже не стоят не месте, некоторые их решения странные, и могут влиять на работу моей программы, в данный момент у меня она работает с утм версии 2562, ошибок не наблюдал.

Буду обновлять ридми, с какой вервией утм ошибок не наблюдаю.

Если вы все таки решили предложить мотивацию, то, будьте добры, подробно описывать вашу ситуацию: утм работают нон стоп или нет, какая версия рутокена, какая версия утм, приложите лог моей программы и лог проблемного утм, будьте готовы предоставить удаленный доступ к машине, где проявляеться эта проблема.

С поступлением вопросов, так же буду обновлять этот раздел ридми...

 
 # Проверено на чистых официальных системах, на всяких сборках работа не гарантирована!

### 2UTM - до 10ти УТМ на одной машине


Обновление 1.1 - релиз можно скачать тут: https://github.com/2UTM/2UTM/releases  
# Для работы программы необходимы права администратора! 


## Часто задаваемые вопросы
1. Как установить?
Если есть уже установленные УТМ, их нужно удалить вручную. Затем открываете программу с правами администратора, меню Операции -> Установить УТМ, далее добавляете УТМы, выбираете порты и указываете установочный файл УТМ. После завершения установки УТМы будут доступны по адресам localhost:<порт, который вы указали при установке>

2. Пропали рутокены, что делать?
Пропажа ридеров (рутокенов) может произойти из за непредвиденной ошибки, после которой прервалась операция.
Чтобы вернуть ридеры, нужно в программе удалить все ридеры рутокена (меню Операции -> Ридеры вне контекста), ридеры рутокенов имеют имя Aktiv Rutoken ECP <номер>, затем в диспетчере устройств отключить все смарт карты (устройства WUDF) и подключить обратно, НЕ УДАЛЯТЬ, а именно отключить все и подключить все. После этого все рутоктены будут видны


По ошибкам и вопросам просьба писать тут:  
GitHub - https://github.com/2UTM/2UTM/issues  
Почта - 2UTM.dev@gmail.com  
Telegram - @regnorr  


Изменения в 1.1  
1. Изменен внешний вид интерфейса, приближен к интерфейсу драйвера рутокен  
2. Изменены все алгоритмы, устранены ошибки с дочерними именами, убраны лишние зависимости, задействовано API смарт карт  
3. Добавлена возможность установки до 10ти УТМ (так как может быть только 10 ридеров для смарт карт)  
4. Добавлена возможность изменения порта для УТМ  
5. Убрана зависимость от порядка токенов в системе  
  
Проверено на Win7, 8, 10 home, pro
  
  
  

Небольшое введение, кто не в курсе что такое УТМ и с чем его едят и почему появилась необходимость в 2UTM...

Все наверное слышали про систему ЕГАИС, через которую торгуют алкоголем (только в России, насколько я знаю). УТМ - это универсальный траспортный модуль, через который чеки отправляются в ФСРАР, так же через него происходит весь документооборот между поставищком и розничными продавцами алкгольной продукции. УТМ разработан ФСРАР, и скачать его можно в личном кабинете ЕГАИС, либо на каком нибудь другом ресурсе, посвященному работе в нем. Так же ФСРАР утверждают что два УТМ на одну машину установить невозможно...



О программе 2UTM...

Где то месяца 2 назад появилась необходимость установить два УТМ на одну машину, так как есть две базы 1С и они должны функционировать одновременно. Сразу появилась идея, так как ФСРАР утверждает что 2 УТМ быть не может, сделать виртульную машину с пробросом портов. Сначала так и сделали, но на практике оказалось очень неудобна и расточительна такая схема работы, и было принято решение исследовать данную проблему и, по возможности, найти программное решение. Так же, так как это писалось для продавцов, нужно было сделать максимально простую и автоматизированную программу, что бы поняли даже бабушки с 3-мя классами церковно приходской. Аналоги я находил, но они топорны и работают через раз.
Соответсвенно была реализована программа, так как она написана на Qt комьюнити, я не имею права продавать ее и закрывать код, поэтому она и здесь... 



Функционал 2UTM...

Для программы нужны административные права, так как присутствует работа с оборудованием и установкой стороних программ (УТМ).
Программа работает только с рутокенами 2.0 (рутокенов 3.0 у меня физически нет), так как для джакарт нельзя так просто скачать sdk (почему, для меня загадка), нужно оставлять заявку и может быть вам его вышлют, так же задачи не стояло, чтобы джакарты тоже поддерживались. Если у вас есть необходимьсть реализовать тот же функционал с джакартами, можете написать здесь на гитхабе или на почту 2UTM.dev@gmail.com. Открыть проект можно через QtCreator...



Спасибо за внимание!

PS
Если есть желание отблагодарить материально - можете написать на почту


Ссылка на гитхаб - https://github.com/2UTM/2UTM

Почта - 2UTM.dev@gmail.com
