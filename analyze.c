struct repo examine_repoFile (const char *reponame, const char *filename) {
	int x = 0;

	struct repo repo = { 0 };

	config_t repoFile;

	if (!config_read_file (&repoFile, filename)) {
		cli_repoFile_syntax (
					reponame,
					config_error_line (&repoFile),
					config_error_text (&repoFile)
					);

		config_destroy (&repoFile);
		repo.status = status_invalid;
		return repo;
	}

	if (!(	// check for missing fields while loading fields
		config_lookup_string (&repoFile, "name", &(repo.name))		&&
		config_lookup_string (&repoFile, "url", &(repo.url))		&&
		config_lookup_string (&repoFile, "burls", &(repo.burls))	&&
		config_lookup_string (&repoFile, "surls", &(repo.surls))	&&
		config_lookup_int (&repoFile, "keys.count", &(repo.keycount))
	)) {
		// if missing, report
		cli_repoFile_missing_main (repoFile);
		config_destroy (&repoFile);
		repo.status = status_invalid;
		return repo;
	}

	// store key meta
	repo.keys = malloc (sizeof (struct repoKey) * repo.keycount));
	assert (repo.keys);
	for (x = 0; x < repo.keycount; x++) {
		char *name, *key, *valid;

		// build lookup paths
		asprintf (&name, "keys.k%i.name", x);
		asprintf (&key, "keys.k%i.key", x);
		asprintf (&valid, "keys.k%i.key", x);
		assert (name && key && valid);

		// validate field existence
		if (!(
			config_lookup_string (&repoFile, name, &(repo.keys[x].name))	&&
			config_lookup_string (&repoFile, key, &(repo.keys[x].key))	&&
			config_lookup_int (&repoFile, valid, &(repo.keys[x].valid))
		)) {
			// if missing, report but delay further action
			cli_repoFile_missing_key (repoFile, name, key, valid);
			repo.status = status_invalid;
		}

		free (name);
		free (key);
		free (valid);
	}

	// if error occurred, clean up and return
	if (repo.status == status_invalid) {
		free (repo.keys);
		config_destroy (&repoFile);
		return repo;
	}

	// otherwise, make hard copies of strings
	//
	// hard copies are delayed till now so that
	// returns can be clean w/o memory issues
	for (x = 0; x < repo.keycount; x++) {	// repo keys
		repo.keys[x].name	= strdup (repo.keys[x].name);
		repo.keys[x].key	= strdup (repo.keys[x].key);
		repo.keys[x].valid	= strdup (repo.keys[x].valid);

		assert (repo.keys[x].name);
		assert (repo.keys[x].key);
		assert (repo.keys[x].valid);
	}
	{	// repo meta
		repo.name	= strdup (repo.name);
		repo.url	= strdup (repo.url);
		repo.burls	= strdup (repo.burls);
		repo.surls	= strdup (repo.surls);

		assert (repo.name);
		assert (repo.url);
		assert (repo.burls);
		assert (repo.surls);
	}

	// clean up
	config.destroy (&repoFile);

	return repo;
}
