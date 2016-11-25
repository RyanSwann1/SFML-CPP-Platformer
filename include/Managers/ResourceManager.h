#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

using ResourceName = std::string;
using ResourceCount = unsigned int;
using FileName = std::string;
using FilePath = std::string;

//hexadecimal numbers
//https://www.dropbox.com/s/qe0688gbpum5ue9/Screenshot%202016-10-05%2021.18.40.png?dl=0
//https://www.dropbox.com/s/t7zj5s4idk6h0tn/Screenshot%202016-10-05%2021.46.07.png?dl=0

template <class T>
class ResourceManager
{
public:
	ResourceManager() {}
	virtual ~ResourceManager() 
	{
		purgeResources();
	}

	//Disallow copying of this class
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	const T* getResource(const std::string& id)
	{
		auto res = m_resources.find(id);
		if (res != m_resources.cend())
		{
			++res->second.second;
			return res->second.first;
		}

		auto iter = m_filePaths.find(id);
		if (iter != m_filePaths.cend())
		{
			const T* resource = loadFromFile(iter->second);
			if (resource)
			{
				m_resources.emplace(id, std::make_pair(resource, 1));
				return resource;
			}
		}
		return nullptr;
	}

	void releaseResource(const std::string& id)
	{
		auto res = m_resources.find(id);
		if (res != m_resources.cend())
		{
			if (res->second.second)
			{
				--res->second.second;
			}
			
			if (!res->second.second)
			{
				removeResource(id);
			}
		}
	}

protected:
	std::unordered_map<ResourceName, std::pair<const T*, ResourceCount>> m_resources;
	std::unordered_map<FileName, FilePath> m_filePaths;

	const virtual T* loadFromFile(const std::string& fileName) const = 0;

	void purgeResources()
	{
		for (auto &i : m_resources)
		{
			delete i.second.first;
			i.second.first = nullptr;
		}
		m_resources.clear();
	}

	void registerFilePath(const std::string& name, const std::string& location)
	{
		if (!m_filePaths.emplace(std::make_pair(name, location)).second)
		{
			std::cerr << "Failed to register file path: '" << name << "'. Location: '" << location << "'\n";
		}
	}
	
private:
	//Note to self - Use find as a pointer, otehrwise copying it would be too expensive for this simple check
	const std::pair<T*, ResourceCount>* find(const std::string& id) const
	{
		const auto iter = m_resources.find(id);
		return (iter != m_resources.cend() ? &iter->second : nullptr);
	}

	void removeResource(const std::string& id)
	{
		auto iter = m_resources.find(id);
		if (iter != m_resources.cend())
		{
			delete iter->second.first;
			iter->second.first = nullptr;
			m_resources.erase(iter);
		}
	}
};