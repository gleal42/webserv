# Run Makefiles of End-to-end integration tests (e2e) and Unit tests (unit)

all:			unit e2e

unit:
					${MAKE} -C unit

e2e:
					${MAKE} -C e2e

clean:
					${MAKE} clean -C e2e
					${MAKE} clean -C unit

fclean:
					${MAKE} fclean -C e2e
					${MAKE} fclean -C unit

re: 			fclean all

.PHONY : 	all clean fclean re
