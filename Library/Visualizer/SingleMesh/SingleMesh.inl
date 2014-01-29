inline SingleMesh::SingleMesh(std::string id, fs::path meshFile) : Visualizer(id), m_meshFile(meshFile) {
}

inline SingleMesh::~SingleMesh() {
}

inline void SingleMesh::init() {
	addProperties();
	addModes();

	m_mesh = std::make_shared<Mesh>();
	if (!Traits::loadFromFile(*m_mesh, m_meshFile.string())) {
		gui()->log()->error("Could not open mesh file \""+m_meshFile.string()+"\"");
	}
	gui()->log()->info("Loaded mesh with "+lexical_cast<std::string>(Traits::numVertices(*m_mesh))+" vertices and "+lexical_cast<std::string>(Traits::numFaces(*m_mesh))+" faces.");

	auto vertices = Traits::vertexPositions(*m_mesh);
	for (const auto& v : vertices) {
		m_bbox.extend(v);
	}

	m_rm = std::make_shared<Rendered::Mesh<Mesh>>(m_mesh, false);

	registerEvents();
}

inline void SingleMesh::render(ShaderProgram& program) {
	float spec = gui()->properties()->get<Range>({"material", "specularity"})->value();
	float refr = gui()->properties()->get<Range>({"material", "refrIndex"})->value();
	program.setUniformVar1f("specularity", spec);
	program.setUniformVar1f("refrIndex", refr);

	bool clipping = gui()->modes()->group("showGroup")->option("showClip")->active();
	if (clipping) {
		Eigen::Vector3f clipNormal = Eigen::Vector3f(0, 0, 1);
		program.setUniformVec3("clipNormal", clipNormal.data());
		program.setUniformVar1f("clipDistance", m_clippingHeight);
		glEnable(GL_CLIP_DISTANCE0);
	}

	m_rm->render(program);

	if (clipping) {
		glDisable(GL_CLIP_DISTANCE0);
	}
}

inline void SingleMesh::addProperties() {
	auto material = gui()->properties()->add<Section>("Material", "material");
	material->add<Range>("Specularity", "specularity")->setDigits(2).setMin(0.0).setMax(1.0).setValue(0.0);
	material->add<Range>("Refraction Index", "refrIndex")->setDigits(2).setMin(1.0).setMax(4.0).setValue(1.3);
}

inline void SingleMesh::addModes() {
	auto renderGroup = gui()->modes()->addGroup("renderGroup");
	renderGroup->addOption("smooth", "Smooth Normal Interpolation", std::string(ICON_PREFIX)+"smooth.png");
	renderGroup->setCallback([&] (std::string option, bool state) { if (option == "smooth") m_rm->setSmoothNormals(state); });

	auto showGroup = gui()->modes()->addGroup("showGroup");
	showGroup->addOption("showClip", "Enable Clipping", std::string(ICON_PREFIX)+"clipping.png");
	auto editGroup = gui()->modes()->addGroup("editGroup");
	editGroup->addOption("editClip", "Modify Clipping Plane", std::string(ICON_PREFIX)+"clipplane.png");
}

inline void SingleMesh::registerEvents() {
	m_clipRangeMin =  std::numeric_limits<float>::infinity();
	m_clipRangeMax = -std::numeric_limits<float>::infinity();
	for (auto vertexId : Traits::vertices(*m_mesh)) {
		float vertexHeight = Traits::vertexPosition(*m_mesh, vertexId)[2];
		if (vertexHeight < m_clipRangeMin) m_clipRangeMin = vertexHeight;
		if (vertexHeight > m_clipRangeMax) m_clipRangeMax = vertexHeight;
	}
	m_clipRangeMin -= 0.1f;
	m_clipRangeMax += 0.1f;
	m_clippingHeight = m_clipRangeMin + 0.5f * (m_clipRangeMax - m_clipRangeMin);
	fw()->events()->connect<void (int, int, int, int)>("LEFT_DRAG", [&] (int, int dy, int, int) {
		if (!gui()->modes()->group("editGroup")->option("editClip")->active()) return;
		m_clippingHeight -= 0.05f * static_cast<float>(dy);
		if (m_clippingHeight < m_clipRangeMin) m_clippingHeight = m_clipRangeMin;
		if (m_clippingHeight > m_clipRangeMax) m_clippingHeight = m_clipRangeMax;
	});
}

inline BoundingBox SingleMesh::boundingBox() const {
	return m_bbox;
}
