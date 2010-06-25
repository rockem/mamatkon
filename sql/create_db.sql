create table recipe (
	id INTEGER PRIMARY KEY,
	name TEXT,
	description TEXT,
	servings INTEGER,
	source TEXT,
	version_id INTEGER
	);
	
create table ingerdiants (
	id INTEGER PRIMARY KEY,
	name TEXT,
	description TEXT
	);

create table recipe_ingrediant (
	recipe_id INTEGER,
	ingrediant INTEGER,
	ingrediant_order INTEGER,
	category_id INTEGER,
	amount INTEGER,
	amount_type_id INTEGER,
	version_id INTEGER
	);
	
create table version_track (
	recipe_id INTEGER,
	version_id INTEGER,
	name TEXT,
	updated INTEGER
	);
	
create table amount_types (
	id INTEGER PRIMARY KEY,
	name TEXT
	);
	
create table recipe_pictures(
	id INTEGER PRIMARY KEY,
	recipe_id INTEGER,
	picture BLOB
	);

	
	
	
	
	
	