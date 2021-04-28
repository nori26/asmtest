#!/bin/bash

STR=""

add_option () {
	grep : ../libasm/$1 > /dev/null 2>&1 && STR+=$2
}

find_mandatory_file () {
	add_option "ft_strlen.s" "-D FT_LEN "
	add_option "ft_strcpy.s" "-D FT_CPY "
	add_option "ft_strcmp.s" "-D FT_CMP "
	add_option "ft_write.s" "-D FT_WR "
	add_option "ft_read.s" "-D FT_RD "
	add_option "ft_strdup.s" "-D FT_DUP "
}

find_bonus_file () {
	add_option "ft_atoi_base_bonus.s" "-D FT_ATOI "
	add_option "ft_list_push_front_bonus.s" "-D FT_PUSH "
	add_option "ft_list_size_bonus.s" "-D FT_SIZE "
	add_option "ft_list_sort_bonus.s" "-D FT_SORT "
	add_option "ft_list_remove_if_bonus.s" "-D FT_REMOVE "

	# add_option "ft_atoi_base.c" "-D FT_ATOI "
	# add_option "ft_list_push_front.c" "-D FT_PUSH "
	# add_option "ft_list_size.c" "-D FT_SIZE "
	# add_option "ft_list_sort.c" "-D FT_SORT "
	# add_option "ft_list_remove_if.c" "-D FT_REMOVE "
}

find_mandatory_file

MANDATORY=$STR

find_bonus_file

if [ "{$STR}" != "{$MANDATORY}" ]; then
	STR+="-D BONUS "
fi

../libasm/make re
../libasm/make bonus
../libasm/make clean
# STR+="ft_atoi_base.c ft_list_push_front.c ft_list_size.c ft_list_sort.c ft_list_remove_if.c "
# STR+=ft_list_sort.c
# STR+="-D SANITIZE -fsanitize=address"
gcc -g -O0 ${STR} asmtest.c ../libasm/libasm.a && ./a.out
rm a.out