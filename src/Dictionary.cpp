/*
 * Dictionary.cpp
 *
 * Copyright (c) 2009 Jonathan Beck All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 */

#include <stdlib.h>
#include <plist/Dictionary.h>
#include <plist/Array.h>

namespace PList
{

Dictionary::Dictionary() : Structure(PLIST_DICT)
{
}

Dictionary::Dictionary(plist_t node) : Structure()
{
    _node = node;
    plist_dict_iter it = NULL;

    char* key = NULL;
    plist_t subnode = NULL;
    plist_dict_new_iter(_node, &it);
    plist_dict_next_item(_node, it, &key, &subnode);
    while (subnode)
    {
	plist_type subtype = plist_get_node_type(subnode);
	switch(subtype)
	{
	    case PLIST_DICT:
		_map[std::string(key)] = new Dictionary(subnode);
		break;
	    case PLIST_ARRAY:
		_map[std::string(key)] = new Array(subnode);
		break;
	    case PLIST_BOOLEAN:
	    case PLIST_UINT:
	    case PLIST_REAL:
	    case PLIST_STRING:
	    case PLIST_DATE:
	    case PLIST_DATA:
	    default:
		_map[std::string(key)] = new Node(subnode);
		break;
	}
	
	subnode = NULL;
	free(key);
	key = NULL;
	plist_dict_next_item(_node, it, NULL, &subnode);
    }
    free(it);
}

Dictionary::Dictionary(Dictionary& d)
{
    for (Dictionary::iterator it = _map.begin(); it != _map.end(); it++)
    {
	plist_free(it->second->GetPlist());
	delete it->second;
    }
    _map.clear();

    _node = plist_copy(d.GetPlist());
    plist_dict_iter it = NULL;

    char* key = NULL;
    plist_t subnode = NULL;
    plist_dict_new_iter(_node, &it);
    plist_dict_next_item(_node, it, &key, &subnode);
    while (subnode)
    {
	plist_type subtype = plist_get_node_type(subnode);
	switch(subtype)
	{
	    case PLIST_DICT:
		_map[std::string(key)] = new Dictionary(subnode);
		break;
	    case PLIST_ARRAY:
		_map[std::string(key)] = new Array(subnode);
		break;
	    case PLIST_BOOLEAN:
	    case PLIST_UINT:
	    case PLIST_REAL:
	    case PLIST_STRING:
	    case PLIST_DATE:
	    case PLIST_DATA:
	    default:
		_map[std::string(key)] = new Node(subnode);
		break;
	}
	
	subnode = NULL;
	free(key);
	key = NULL;
	plist_dict_next_item(_node, it, NULL, &subnode);
    }
    free(it);
}

Dictionary& Dictionary::operator=(const Dictionary& d)
{
    for (Dictionary::iterator it = _map.begin(); it != _map.end(); it++)
    {
	plist_free(it->second->GetPlist());
	delete it->second;
    }
    _map.clear();

    _node = plist_copy(d.GetPlist());
    plist_dict_iter it = NULL;

    char* key = NULL;
    plist_t subnode = NULL;
    plist_dict_new_iter(_node, &it);
    plist_dict_next_item(_node, it, &key, &subnode);
    while (subnode)
    {
	plist_type subtype = plist_get_node_type(subnode);
	switch(subtype)
	{
	    case PLIST_DICT:
		_map[std::string(key)] = new Dictionary(subnode);
		break;
	    case PLIST_ARRAY:
		_map[std::string(key)] = new Array(subnode);
		break;
	    case PLIST_BOOLEAN:
	    case PLIST_UINT:
	    case PLIST_REAL:
	    case PLIST_STRING:
	    case PLIST_DATE:
	    case PLIST_DATA:
	    default:
		_map[std::string(key)] = new Node(subnode);
		break;
	}
	
	subnode = NULL;
	free(key);
	key = NULL;
	plist_dict_next_item(_node, it, NULL, &subnode);
    }
    free(it);
}

Dictionary::~Dictionary()
{
    for (Dictionary::iterator it = _map.begin(); it != _map.end(); it++)
    {
	plist_free(it->second->GetPlist());
	delete it->second;
    }
    _map.clear();
}

Node* Dictionary::operator[](std::string& key)
{
    return _map[key];
}

Dictionary::iterator Dictionary::Begin()
{
    return _map.begin();
}

Dictionary::iterator Dictionary::End()
{
    return _map.end();
}

void Dictionary::Insert(std::string& key, Node* node)
{
    if (node)
    {
	plist_dict_insert_item(_node, key.c_str(), node->GetPlist());
	delete _map[key];
	_map[key] = node;
    }
}

void Dictionary::Remove(Node* node)
{
    if (node)
    {
	char* key = NULL;
	plist_dict_get_item_key(node->GetPlist(), &key);
	plist_dict_remove_item(_node, key);
	std::string skey = key;
	free(key);
	delete node;
    }
}

void Dictionary::Remove(std::string& key)
{
	plist_dict_remove_item(_node, key.c_str());
	delete _map[key];
}

};
