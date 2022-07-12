/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gleal <gleal@student.42lisboa.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/12 15:59:39 by gleal             #+#    #+#             */
/*   Updated: 2022/07/12 16:07:17 by gleal            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CONNECTION_H__
# define __CONNECTION_H__

class Connection
{
	Connection();
	Connection(const Connection & src);
	~Connection();
	Connection &operator=(const Connection & src);
};

#endif
