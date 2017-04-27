#include <stdio.h>
#include <cstl/cmap.h>

int main()
{
	map_t* map = create_map(int,int*);
	map_init(map);
	int a = 1;
	int b = 2;
	(int*)map_at(map,1) = &a;
	(int*)map_at(map,2) = &b;

	map_iterator_t it;
	printf("%d\n",map_size(map));
	for (it = map_begin(map); !iterator_equal(it,map_end(map)); it = iterator_next(it))
	{
		printf("key = %d, value = %d\n",*(int*)pair_first(iterator_get_pointer(it)), *(int*)pair_second(iterator_get_pointer(it)));
	}

	a = 3;

	for (it = map_begin(map); !iterator_equal(it,map_end(map)); it = iterator_next(it))
	{
		printf("key = %d, value = %d\n",*(int*)pair_first(iterator_get_pointer(it)), *(int*)pair_second(iterator_get_pointer(it)));
	}
	

}
