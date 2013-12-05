<?php

function quizsolved()
{
  global $pguser;
  if (isset($pguser))
      record_quiz_attempt($pguser,"p_mod1_2","pass");
}

$browser_title                = _("Moderate Proofreading Quiz");
$intro_title                  = _("Moderate Proofreading Quiz, page 2");
$initial_instructions         = _("Try to correct the text on the bottom left, so it matches the text in the image above following the Proofreading Guidelines. When done click 'check'.");
$ocr_text                     = "364   ALICE ORVILLE.\n\n\"When folks can't do as they will, they must do\nas they can, I've heard say.\"\n\nThus we leave our adventurers and return north-\neast to the land from which they are receding. We\ndid n't know what else to do here, reader, for we\nwere quite as tired of the characters as you wore,\nand wanted to get them off our hands in some\nway. * * * A few people think E--e can tell\nstories tolerably well. But she can't, reader! We";
$solutions                    = array("\n\"When folks can't do as they will, they must do\nas they can, I've heard say.\"\n\nThus we leave our adventurers and return north-*east\nto the land from which they are receding. We\ndidn't know what else to do here, reader, for we\nwere quite as tired of the characters as you were,\nand wanted to get them off our hands in some\nway. * * * A few people think E--e can tell\nstories tolerably well. But she can't, reader! We");
$solved_message               = "<h2>" . _("Quiz successfully solved") . "</h2>\n" .
                                    "<p>" . _("Congratulations, no errors found!") . "</p>\n" .
                                    "<h3>" . _("Handy Fact") . "</h3>\n" .
                                    "<p>" . _("Enlarging the image in the proofreading interface can often help you to identify characters that may seem unclear at first.  You can use the +25% button repeatedly in the standard interface, or simply enter the size you want, such as 200%, in the enhanced interface.") . "</p>";


// error messages

$messages["wore"] = array(
    "message_text" => "<h2>" . _("Scanno") . "</h2>\n" .
        "<p>" . _("There is still a 'scanno': an '<tt>o</tt>' in the text where there is an '<tt>e</tt>' in the image.") . "</p>",
    "hints" => array(),
);
$messages["asterspace"] = array(
    "message_text" => "<h2>" . _("Punctuation spacing") . "</h2>\n" .
        "<p>" . _("Leave spaces between asterisks if they appear that way in the original book.") . "</p>",
    "hints" => array(),
);
$messages["asterchanged"] = array(
    "message_text" => "<h2>" . _("Asterisks") . "</h2>\n" .
        "<p>" . _("If the original book uses an asterisk, keep this as the author wrote it.") . "</p>",
    "hints" => array(),
);
$messages["para"] = array(
    "message_text" => "<h2>" . _("Paragraph spacing") . "</h2>\n" .
        "<p>" . _("Leave a single blank line between paragraphs, even if there is an extra gap in the image.") . "</p>",
    "hints" => array(),
);


// error checks

$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("364", "ALICE"), "case_sensitive" => TRUE, "error" => "P_header");
$tests[] = array("type" => "expectedtext", "searchtext" =>  array("\n\"When"), "case_sensitive" => TRUE, "error" => "P_paratop");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("wore"), "case_sensitive" => TRUE, "error" => "wore");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("did n"), "case_sensitive" => TRUE, "error" => "P_contracspace");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("**"), "case_sensitive" => TRUE, "error" => "asterspace");
$tests[] = array("type" => "expectedtext", "searchtext" =>  array("* * *"), "case_sensitive" => TRUE, "error" => "asterchanged");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("-\n"), "case_sensitive" => TRUE, "error" => "P_eolhyphen");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("northeast", "north-east"), "case_sensitive" => TRUE, "error" => "P_eolhyphenaster");
$tests[] = array("type" => "longline", "lengthlimit" => 70, "error" => "P_longline");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("---"), "case_sensitive" => TRUE, "error" => "P_em_long");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("-- ", " --"), "case_sensitive" => TRUE, "error" => "P_dashneedspace");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("<i>", "</i>"), "case_sensitive" => FALSE, "error" => "P_italmarkup");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("<tb>", "Illustration"), "case_sensitive" => FALSE, "error" => "P_tbmarkup");
$tests[] = array("type" => "forbiddentext", "searchtext" =>  array("\n\n\n"), "case_sensitive" => TRUE, "error" => "para");

// vim: sw=4 ts=4 expandtab