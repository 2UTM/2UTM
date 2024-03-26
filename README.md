### Буду рад пожертвованиям на дальнейшую разработку, спасибо!
  
  # Проверено на чистых официальных системах, на всяких сборках работа не гарантирована!

Windows Defender может ругаться, что тут вирус троян, но это из за того, что exe файл не подписан

### Коды ошибок взяты из WinAPI, список ошибок можно посмотреть здесь - https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-

### 2UTM - до 10ти УТМ на одной машине


Обновление 1.3 - релиз можно скачать тут: https://github.com/2UTM/2UTM/releases  
# Для работы программы необходимы права администратора! 

### Issue закрываю, если за месяц в них не было активности


## Часто задаваемые вопросы
1. Как установить?
Если есть уже установленные УТМ, их нужно удалить вручную. Затем открываете программу с правами администратора, меню Операции -> Установить УТМ, далее добавляете УТМы, выбираете порты и указываете установочный файл УТМ. После завершения установки УТМы будут доступны по адресам localhost:<порт, который вы указали при установке>.

2. Пропали рутокены, что делать?
Пропажа ридеров (рутокенов) может произойти из за непредвиденной ошибки, после которой прервалась операция.
Чтобы вернуть ридеры, нужно в программе удалить все ридеры рутокена (меню Операции -> Ридеры вне контекста), ридеры рутокенов имеют имя Aktiv Rutoken ECP <номер>, затем в диспетчере устройств отключить все смарт карты (устройства WUDF) и подключить обратно, НЕ УДАЛЯТЬ, а именно отключить все и подключить все. После этого все рутоктены будут видны.


Заметьте, что у программы нет никакой лицензии, то есть это, по сути, экспериментальное ПО. Cтабильность никто не гарантирует, если вам кто то сказал, что все работает стабильно, можете пнуть его ногой в живот. Так же вы можете использовать это ПО, изменять и внедрять где и куда угодно, но на свой страх и риск.

У УТМ есть еще два служебных порта - 8192 и 8194. Программа меняет их для каждого УТМ - первый уменьшает на 1, второй увеличивает на 1. Если вы хотите поменять порты для веб интерфейса, то учтите, что при установке 10 УТМ порты 8183 - 8192 и 8194 - 8203 будут заняты.

Программа не контролирует работу УТМ, она создает условия, чтобы несколько УТМ смогли работать.
Ограничение в 10 УТМ, потому что в ОС Windows, начиная с Windows 8, одновременно может быть только 10 ридеров для смарт карт.

Что нового в версии 1.3:
1. Добавлена служба 2UTM_service, с помощью нее реализован автозапуск УТМов при запуске компьютера.
2. Исправлено UB при запуске УТМов, была гонка за ресурс.
3. Исправлена небольшая ошибка с выделением токена в листбоксе.
4. Исправлена небольшая ошибка с логом.
  
Проверено на Win7, 8, 10 home, pro. На серверных платформах и с прокидкой через терминальник не проверял.
  
  

Небольшое введение, кто не в курсе что такое УТМ и с чем его едят и почему появилась необходимость в 2UTM...

Все наверное слышали про систему ЕГАИС, через которую торгуют алкоголем (только в России, насколько мне известно). УТМ - это универсальный траспортный модуль, через который чеки отправляются в ФСРАР, так же через него происходит весь документооборот между поставищком и розничными продавцами алкгольной продукции. УТМ разработан ФСРАР, и скачать его можно в личном кабинете ЕГАИС, либо на каком нибудь другом ресурсе, посвященному работе в нем. Так же ФСРАР утверждают что два УТМ на одну машину установить невозможно...



О программе 2UTM...

Появилась необходимость установить два УТМ на одну машину, так как есть две базы 1С и они должны функционировать одновременно. Сразу появилась идея, так как ФСРАР утверждает что 2 УТМ быть не может, сделать виртульную машину с пробросом портов. Сначала так и сделали, но на практике оказалось очень неудобна и расточительна такая схема работы, и было принято решение исследовать данную проблему и, по возможности, найти программное решение. Так же, так как это писалось для продавцов, нужно было сделать максимально простую и автоматизированную программу, что бы поняли даже бабушки с 3-мя классами церковно приходской. Аналоги я находил, но они топорны и работают через раз.



Функционал 2UTM...

Для программы нужны административные права, так как присутствует работа с оборудованием и установкой стороних программ (УТМ).
Программа была проверена только с рутокенами 2.0 (рутокенов 3.0 у меня физически нет), так как для джакарт нельзя так просто скачать sdk (почему, для меня загадка), нужно оставлять заявку и может быть вам его вышлют, так же задачи не стояло, чтобы джакарты тоже поддерживались. Если у вас есть необходимьсть реализовать тот же функционал с джакартами, можете сообщить мне об этом, обсудим.



Спасибо за внимание!


Разработчик: Королев Сергей.

По ошибкам и вопросам просьба писать тут:  
GitHub - https://github.com/2UTM/2UTM/issues  
Почта - 2UTM.dev@gmail.com  
Telegram - @regnorr
