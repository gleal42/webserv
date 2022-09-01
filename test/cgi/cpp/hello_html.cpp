/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hello_html.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/31 15:25:18 by gleal             #+#    #+#             */
/*   Updated: 2022/07/31 15:31:44 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>

int main( void )
{
	std::cout << "<!DOCTYPE html>" << std::endl
	<< "<html>" << std::endl
	<< "<body>" << std::endl
	<< "<h1>Basic CGI Script</h1>" << std::endl
	<< "<p>My first paragraph.</p>" << std::endl
	<< "</body>" << std::endl
	<< "</html>";
}
