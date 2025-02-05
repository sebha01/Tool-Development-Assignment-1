#include "Scene.h"

void Scene::load()
{
	rootModel.children.clear();
	CurrentObject = 3;
	selected_model = nullptr;
	ti::XMLDocument doc;
	doc.LoadFile("..\\..\\Resources\\Levels\\Scene01.xml");
	if (doc.Error())
		std::cerr << "Error parsing XML: " << doc.ErrorStr() << std::endl;
	else
	{
		ti::XMLElement* scene = doc.RootElement();
		ti::XMLElement* model = scene->FirstChildElement("Model");
		if (model) //the scene has at lease 1 valid model so load it
			loadModel(model, &rootModel.children, &CurrentObject);
	}
}

void Scene::loadModel(ti::XMLElement* model, vector<Model>* modelList, int* CurrentObject)
{
	int count = 0;
	while (model)
	{
		string path = model->FirstChildElement("PathName")->GetText();
		string data = model->FirstChildElement("Data")->GetText();
		fs::path pathName(path);
		cout << "Loading Model"<<*CurrentObject<<" PathName: " << pathName << " Data: " << data << endl;
		modelList->push_back(*new Model(pathName.string(), pathName.filename().string(), data));
		(*modelList)[count].ID = (*CurrentObject);
		(*CurrentObject)++;


		ti::XMLElement* modelChild = model->FirstChildElement("Model");
		if (modelChild)
			loadModel(modelChild, &((*modelList)[count].children), CurrentObject);

		cout << "Loaded:" << pathName.string() << endl;
		model = model->NextSiblingElement("Model");
		count++;
	}
}


bool Scene::selectModelByID(Model* model, int CurrentObjectID)
{
	if (model->ID == CurrentObjectID)
	{
		selected_model = model;
		cout << "Selected Model ID = " << selected_model->ID << ", Name = " << selected_model->name<< endl;
		return true;
	}
	else
	{
		for (int i = 0; i < model->children.size(); i++)
			if(selectModelByID(&model->children[i], CurrentObjectID))
				return true;
	}
	return false;
}


void Scene::saveModel(Model* model, ofstream* outfile)
{
	if(model->name!="SceneRoot")
	{ 
		cout << "Saving model" << endl;
		*outfile << "<Model>";
		cout << model->toXML() << endl;
		*outfile << model->toXML() << endl;
	}
	if (model->children.size() > 0)
		for (int i = 0; i < model->children.size(); i++)
			saveModel(&model->children[i], outfile);
	if (model->name != "SceneRoot")
		*outfile << "</Model>";
}


void Scene::save()
{
	ofstream outfile;
	cout << "Opening file" << endl;
	outfile.open("..\\..\\Resources\\Levels\\Scene01.xml");

	if (outfile) {
		//Scene start tag
		cout << "Saving scene" << endl;
		outfile << "<Scene>" << endl;
		saveModel(&rootModel, &outfile);
		//Scene end tag
		outfile << "</Scene>" << endl;
		outfile.close();
	}
	cout << "Done saving" << endl;
}


void Scene::addNavPoint()
{
	//Add a newIf navSet.navModel is null then call navSet.init()
	if (navSet.navModel == nullptr)
	{
		navSet.init();
	}

	//• Add NavPoint to navSet.navPoints vector
	navSet.navPoints.push_back(*new NavPoint("Nav" + to_string(255 - currentNavPoint)));

	//•Set selected_model to null
	selected_model = nullptr;

	//•Set selected_navPoint to the new NavPoint
	int size = navSet.navPoints.size();
	selected_NavPoint = &(navSet.navPoints[size - 1]);

	//• Set the selected_navPoint->ID to CurrentNavPoint
	selected_NavPoint->ID = currentNavPoint;

	//•Decrement CurrentNavPoint
	currentNavPoint--;
}

bool Scene::selectNodeByID(int CurrentNodeID)
{
	//For each navPoint in NavSets
	for (NavPoint navPoint : navSet.navPoints)
	{
		//If navPoint ID == urrentNodeID
		if (navPoint.ID == CurrentNodeID)
		{
			//Set selected_navPoint to navPoint
			selected_NavPoint = &navPoint;
			//Set selected_model to null
			selected_model = nullptr;

			return true;
		}
	}

	return false;
}

void Scene::saveNavSet()
{
	//Open an XML file : "..\\..\\Resources\\Levels\\NavSet01.xml" for writing
	ofstream outfile;
	cout << "Opening file" << endl;
	outfile.open("..\\..\\Resources\\Levels\\NavSet01.xml");

	if (outfile) {
		//Scene start tag
		cout << "Saving navSets" << endl;
		//write the "<NavSet>“ start tag
		outfile << "<NavSet>" << endl;

		//for each navPoint
		for (NavPoint& navPoint : navSet.navPoints)
		{
			//write the "<NavPoint>“start tag
			outfile << "<NavPoint>" << endl;
			//Call the navPoint[].toXML() method to write its data
			outfile << navPoint.toXML() << endl;
			//write the "</NavPoint>" end tag
			outfile << "</NavPoint>" << endl;
		}
		//write the "</NavSet>" end tag
		outfile << "</NavSet>" << endl;
		//Close the xml file
		outfile.close();
	}
	else  
	{
		//handle error
		 
		cerr << "Error parsing XML: " << endl;
	}
}

void Scene::loadNavSet()
{
	// Set CurrentNavPoint to 255
	currentNavPoint = 255;
	// Set selected_navPoint to nullptr
	selected_NavPoint = nullptr;

	// Open an XML file for reading
	ti::XMLDocument doc;
	doc.LoadFile("..\\..\\Resources\\Levels\\NavSet01.xml");

	// Check if the file was loaded correctly
	if (doc.Error())
	{
		// Handle error if XML file failed to load
		std::cerr << "Error parsing XML: " << doc.ErrorStr() << std::endl;
	}
	else
	{
		// Get the root element <NavSet>
		ti::XMLElement* navSetElement = doc.RootElement();

		// Get the first <NavPoint> element
		ti::XMLElement* firstNavPointElement = navSetElement->FirstChildElement("NavPoint");

		// Iterate through each <NavPoint> element
		while (firstNavPointElement)
		{
			// Ensure the Name and Data elements exist before trying to access them
			ti::XMLElement* nameElement = firstNavPointElement->FirstChildElement("Name");
			ti::XMLElement* dataElement = firstNavPointElement->FirstChildElement("Data");

			// Check if both Name and Data elements exist
			if (nameElement && dataElement)
			{
				// Extract the name of the NavPoint
				string NPname = nameElement->GetText();

				// Extract the Data string
				string NPdata = dataElement->GetText();

				// Create a new NavPoint and set its name
				NavPoint* np = new NavPoint(NPname);

				// Set position, rotation, and scale using the extracted Data
				np->setData(NPdata);

				// Set the ID of the NavPoint
				np->ID = currentNavPoint;

				// Add the NavPoint to the navSet's vector of NavPoints
				navSet.navPoints.push_back(*np);

				// Decrement the CurrentNavPoint ID for the next NavPoint
				currentNavPoint--;
			}
			else
			{
				// Handle the case where Name or Data element is missing
				std::cerr << "Missing Name or Data element in NavPoint" << std::endl;
			}

			// Move to the next sibling <NavPoint> element
			firstNavPointElement = firstNavPointElement->NextSiblingElement("NavPoint");
		}

		// Optionally, you can log the completion of loading
		std::cout << "Loading completed" << std::endl;
	}
}