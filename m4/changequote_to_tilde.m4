/**
 * @file changequote_to_tilde.m4
 *
 * Change quoting from standard m4 to my own
 * style with tilde used in this project.
 * I couldn't use single quote as it's the character
 * used to declare character literals.
 * My IDE - eclipse - doesn't get single quotation
 * good - it mismatches and indexes everything and
 * it's not livable.
 * So I made the decision to move to tilde character 
 * as the ending quotation character.
 * 
 * This file is most probably included from lib.m4
 */
m4_changequote(`[',`]')
m4_changequote([`],[~])
