<?php

function quizsolved()
{
  global $pguser;
  if (isset($pguser))
      record_quiz_attempt($pguser,"p_mod1_4","pass");
}

$browser_title                = _("Moderate Proofreading Quiz");
$intro_title                  = _("Moderate Proofreading Quiz, page 4");
$initial_instructions         = _("Try to correct the text on the bottom left, so it matches the text in the image above following the Proofreading Guidelines. When done click 'check'.");
$ocr_text                     = "And the meteor on the grave,\nAnd the wisp on the morass;6\nWhen the falling stars are shooting,\nAnd the answer'd owls are hooting.\n\n6[Manfred was done into Italian by a translator\n\"who was unable to find in the dictionaries . . .\nany other signification of the 'wisp' of this line\nthan 'a bundle of straw.\"' Byron offered him\n2OO francs if he would destroy the MS. He at\nfirst refused, but finally signed the agree-\nment.--Life, p. 3l6, note.]";
$solutions                    = array("And the meteor on the grave,\nAnd the wisp on the morass;[6]\nWhen the falling stars are shooting,\nAnd the answer'd owls are hooting,\n\n6 [Manfred was done into Italian by a translator\n\"who was unable to find in the dictionaries ...\nany other signification of the 'wisp' of this line\nthan 'a bundle of straw.'\" Byron offered him\n200 francs if he would destroy the MS. He at\nfirst refused, but finally signed the agreement.--Life,\np. 316, note.]");
$solved_message               = "<h2>" . _("Quiz successfully solved") . "</h2>\n" .
                                    "<p>" . _("Congratulations, no errors found!") . "</p>\n" .
                                    "<h3>" . _("Handy Fact") . "</h3>\n" .
                                    "<p>" . _("When the text is rewrapped during post-processing, the end of a line is converted into a space.  This means that during proofreading, anything that should have a space after it (such as an ellipsis) can be left at the end of a line.") . "</p>";


// error messages

$messages["onescanno"] = array(
    "message_text" => "<h2>" . _("Scanno") . "</h2>\n" .
        "<p>" . _("There is still a 'scanno': an '<tt>l</tt>' (lower case L) in the text where there is a '<tt>1</tt>' (number one) in the image.") . "</p>",
    "hints" => array(),
);
$messages["zeroscanno"] = array(
    "message_text" => "<h2>" . _("Scanno") . "</h2>\n" .
        "<p>" . _("There is still a 'scanno': an '<tt>O</tt>' (capital letter o) in the text where there is a '<tt>0</tt>' (zero) in the image.") . "</p>",
    "hints" => array(),
);
$messages["quotescanno"] = array(
    "message_text" => "<h2>" . _("Scanno") . "</h2>\n" .
        "<p>" . _("There is still a 'scanno' in the text: adjacent quotation marks were OCR'd incorrectly.") . "</p>",
    "hints" => array(),
);
$messages["eolellips"] = array(
    "message_text" => "<h2>" . _("Ellipsis") . "</h2>\n" .
        "<p>" . _("An ellipsis can appear at the end of a line; there is no need to bring up a word from the following line.") . "</p>",
    "hints" => array(),
);
$messages["fnsuper"] = array(
    "message_text" => "<h2>" . _("Footnote marker") . "</h2>\n" .
        "<p>" . _("There is no need for superscript markup for footnote markers.  Instead, the marker in the text should be surrounded by square brackets, like this: <tt>[6]</tt>.") . "</p>",
    "hints" => array(),
    "guideline" => "footnotes",
);
$messages["nobrackets"] = array(
    "message_text" => "<h2>" . _("Footnote marker brackets") . "</h2>\n" .
        "<p>" . _("In the text, the footnote marker should be in square brackets, like this: <tt>[6]</tt>.") . "</p>",
    "hints" => array(),
    "guideline" => "footnotes",
);
$messages["fnmarkerspace"] = array(
    "message_text" => "<h2>" . _("Footnote marker spacing") . "</h2>\n" .
        "<p>" . _("Put the footnote marker right next to the word (or punctuation mark) being footnoted, like this: <br><tt>word[6] word</tt>.") . "</p>",
    "hints" => array(),
    "guideline" => "footnotes",
);
$messages["bracket"] = array(
    "message_text" => "<h2>" . _("Bracket removed") . "</h2>\n" .
        "<p>" . _("Leave the punctuation as the author wrote it, including any brackets in the footnotes.") . "</p>",
    "hints" => array(),
);
$messages["aposchange"] = array(
    "message_text" => "<h2>" . _("Primary Rule") . "</h2>\n" .
        "<p>" . _("The primary rule of proofreading is <i>\"Don't change what the author wrote!\"</i> Please keep the spelling and punctuation as the author wrote it, including any contractions.") . "</p>",
    "hints" => array(),
);


// error checks

$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("hooting."), "case_sensitive" => TRUE, "error" => "P_commascanno");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("3l6"), "case_sensitive" => TRUE, "error" => "onescanno");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("O"), "case_sensitive" => TRUE, "error" => "zeroscanno");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("straw.\"'"), "case_sensitive" => TRUE, "error" => "quotescanno");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array(". . ."), "case_sensitive" => TRUE, "error" => "P_ellspaced");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("any\n"), "case_sensitive" => TRUE, "error" => "eolellips");
$tests[] = array("type" => "expectedtext", "searchtext" =>  array("ies ..."), "case_sensitive" => TRUE, "error" => "P_midellips");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("e-\n"), "case_sensitive" => TRUE, "error" => "P_eolhyphen");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("\nagreement"), "case_sensitive" => TRUE, "error" => "P_hyphenlower");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("agree-ment"), "case_sensitive" => TRUE, "error" => "P_hyphenleft");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("--\n"), "case_sensitive" => TRUE, "error" => "P_eoldash");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("\n--"), "case_sensitive" => TRUE, "error" => "P_soldash");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("-- ", " --"), "case_sensitive" => TRUE, "error" => "P_spacedemdash");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("---"), "case_sensitive" => TRUE, "error" => "P_em_long");
$tests[] = array("type" => "longline", "lengthlimit" => 65, "error" => "P_longline");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("<i>", "</i>"), "case_sensitive" => FALSE, "error" => "P_italmarkup");
$tests[] = array("type" => "expectedtext", "searchtext" =>  array("[Manf"), "case_sensitive" => TRUE, "error" => "bracket");
$tests[] = array("type" => "expectedtext", "searchtext" =>  array("note.]"), "case_sensitive" => TRUE, "error" => "bracket");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("hooting,\n6"), "case_sensitive" => TRUE, "error" => "P_fnspace");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("morass;*", "morass;[*]", "morass;[A]", "morass; *", "morass; [*]"), "case_sensitive" => TRUE, "error" => "P_fnmarkermissing");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("^6"), "case_sensitive" => TRUE, "error" => "fnsuper");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("morass;6"), "case_sensitive" => TRUE, "error" => "nobrackets");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("morass; [6]"), "case_sensitive" => TRUE, "error" => "fnmarkerspace");
$tests[] = array("type" => "multioccurrence", "searchtext" => "[6]", "case_sensitive" => TRUE, "error" => "P_fnbottombrackets");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("6[M"), "case_sensitive" => TRUE, "error" => "P_fnbottomnospace");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("Footnote"), "case_sensitive" => FALSE, "error" => "P_fnmarkup");
$tests[] = array("type" => "expectedtext", "searchtext" =>  array("\n6 [M"), "case_sensitive" => TRUE, "error" => "P_fnbottommarker");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("\n "), "case_sensitive" => TRUE, "error" => "P_indent");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("/*", "*/"), "case_sensitive" => TRUE, "error" => "P_poemmarkup");
$tests[] = array("type" => "expectedtext", "searchtext" =>  array("answer'd"), "case_sensitive" => TRUE, "error" => "aposchange");

// vim: sw=4 ts=4 expandtab