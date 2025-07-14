# 사용 방법
로컬에서는 `testlib.h`와 같은 방법으로 사용하면 됩니다.  
Codeforces Polygon에서는 
Additional Files 탭에 가서 Resource Files에 graphGen.h를 업로드하면 `#include "graphGen.h"`를 통해 사용할 수 있습니다.

## 함수 목록
- 표기법은 `function_name(arguments) -> return_type` 입니다.
- `[인자]`는 그 인자가 생략 가능함을 의미합니다.
### 정적 함수
`Graph::(함수)` 와 같이 호출할 수 있는 함수들입니다.  
그래프를 반환하는 함수들의 경우 `Graph` 타입이 반환됩니다.

설명 추가 예정

1. `tree(n, [elongation]) -> Graph`
2. `cactus(n, tree_n, cycle_cnt, [elongation]) -> Graph`
3. `connected_graph(n, m, [elongation]) -> Graph`
4. `rand_graph(n, m) -> Graph`
5. `line_graph(n, [first_node, last_node]) -> Graph`
6. `complete_graph(n) -> Graph`
7. `star_graph(n, [root]) -> Graph`
8. `skeleton_graph(n) -> Graph`
9. `halin_graph(n, [elongation]) -> Graph`
10. `tree_without_degree_of_2(n, [elongation]) -> Graph`
11. `distribute_natural_numbers(sum, count) -> vector<int>`
### 멤버 함수
1. `find_group(a) -> int`
2. 추가 예정

