# Тестовое задание "Многопоточность"
Реализовать "многопоточный движок эффектов" на основе предоставленного "фреймворка".
- По клику из точки в разные стороны с различной скорость вылетает 64 частицы.
- Они медленно падают вниз.
- Через 1.5 секунды каждая частица с заданной вероятностью либо гаснет, либо превращается в такой же "взрыв".
- Частицы за пределами экрана удаляются.

## Что важно в реализации:
- Все расчёты движения частиц должны происходить в отдельном потоке или потоках.
  Частота обновления частиц не должна зависеть от частоты кадров основного потока.
  На один кадр отрисовки может приходиться несколько обновлений эффектов и наоборот,
    если обновление занимает больше одного кадра, это не должно влиять на частоту рендера.
- В процессе работы решение не должно выделять или освобождать память. Вся память должна быть выделена заранее и освобождена в конце.
- Максимально количество эффектов - 2048 (по 64 частицы каждый). При превышении этого бюджета движок не должен падать или выделять новой памяти.
- Для отрисовки используйте функцию platform::drawPoint.
  Это задача про многопоточность и структуры данных, а не про графику.

## Что НЕ важно в реализации:
- Визуальная красота не оценивается. 
- В рамках задачи не предполагается конфигурация эффектов без пересборки, настройки можно оставить в коде.

## Формальные требования:
- В предоставленном решении запрещается менять main.cpp и файлы из папок GLUT и GL. Остальные файлы менять можно, в том числе создавать новые.
- Интерфейс который "фреймворк" ожидает от решения определён в файле test.h, в неймспейсе test.
- Запрещается в решении использовать любые api и библиотки кроме STD и platform::drawPoint, в том числе: WinApi, pthread, QT, MFC, GLUT, GL, DirectX.
- Нельзя использовать партикловые движки или готовые решения из интернета.
- При разработке на основе каких-либо статей их необходимо указать в документации.
- Решения принимаются под Microsoft Visual Studio 2022.
- В качестве решения ожидается проект Visual Studio с исходным кодом.

## Что будет оцениваться:
- Эффективность структур данных с точки зрения памяти и скорости.
- Эффективность использованных алгоритмов.
- Сопровождаемость, читаемость и аккуратность кода.
- Минимизация времени блокировок потоков. В идеале хочется увидеть не блокирующие алгоритмы.
- Производительность. Не строго. Просто не должно быть очевидных тормозов.

## Документация:
Буквально 3-10 предложений.
Использовались ли какие-то статьи, 
 чем руководствовались при принятии решений
 и общее описание реализации.
Если есть интересные решения - описать.
# multithreadingTestTask
