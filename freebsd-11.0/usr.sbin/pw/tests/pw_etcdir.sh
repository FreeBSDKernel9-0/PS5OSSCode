# $FreeBSD: releng/11.0/usr.sbin/pw/tests/pw_etcdir.sh 274632 2014-11-17 15:44:00Z brd $

# When the '-V directory' option is provided, the directory must exist
atf_test_case etcdir_must_exist
etcdir_must_exist_head() {
	atf_set "descr" "When the '-V directory' option is provided, the directory must exist"
}

etcdir_must_exist_body() {
	local fakedir="/this_directory_does_not_exist"
	atf_check -e inline:"pw: no such directory \`$fakedir'\n" \
		-s exit:72 -x pw -V ${fakedir} usershow root
}

atf_init_test_cases() {
	atf_add_test_case etcdir_must_exist
}

