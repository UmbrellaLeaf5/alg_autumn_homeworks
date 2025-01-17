# Задача: можно ли объединить список слов в цепочку

Дан набор слов. Требуется определить, можно ли расположить эти слова в таком порядке, чтобы последняя буква каждого слова совпадала с первой буквой следующего слова, образуя замкнутую цепочку.

Например, слова “for”, “rig”, “geek”, “kaf” можно соединить в цепочку (for -> rig -> geek -> kaf -> for), в то время как для слов “aab”, “abb” это невозможно.

## Входные данные:

* Первая строка содержит целое число `N` (`N` > 0) — количество слов.
* Следующая строка содержит `N` строковых литералов, описывающих набор **слов**.

## Выходные данные:

* Выходные данные представляют собой строку, содержащую `Yes`, если слова образуют цепочку и `No`, если нет.

## Решение:

Для решения задачи используется подход, основанный на теории графов.  Набор слов преобразуется в ориентированный граф следующим образом:

1. **Построение графа:**  Каждая буква английского алфавита (a-z) представляет собой вершину графа.  Для каждого слова создаётся ориентированное ребро, соединяющее первую букву слова с его последней буквой.  Например, для слова "for" будет создано ребро от вершины 'f' к вершине 'r'.

2. **Проверка на Эйлеров цикл:** Задача сводится к проверке существования Эйлерова цикла в построенном графе. Эйлеров цикл — это циклический путь, проходящий через каждое ребро графа ровно один раз.  Для существования эйлерова цикла необходимо и достаточно выполнение следующих условий:

    * **Степень вершин:**  В ориентированном графе для каждой вершины количество входящих ребер должно быть равно количеству исходящих ребер.
    * **Связность:** Граф должен быть сильно связным, то есть из любой вершины должна быть достижима любая другая вершина, двигаясь по ребрам.

3. **Алгоритм проверки:** Алгоритм реализует проверку условий существования эйлерова цикла:

    * **Подсчет степеней:**  Для каждой вершины подсчитываются входящая и исходящая степени. Если хотя бы для одной вершины эти степени не равны, эйлеров цикл невозможен.
    * **Проверка на связность:** Используется алгоритм поиска в глубину (Depth-First Search, DFS). Начинается обход с произвольной вершины, имеющей исходящее ребро.  Если после DFS не все вершины с ненулевой степенью были посещены, значит граф не сильно связный, а значит, и эйлерова цикла нет.

Если оба условия выполнены, алгоритм возвращает "Yes", в противном случае — "No".
