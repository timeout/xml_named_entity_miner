#include <QRegExp>
#include <QString>
#include <string>
#include "gtest/gtest.h"

TEST( RegularExpressionTest, AttributeTest ) {
    const QString str{" id=\"10\" "};
    QRegExp xpr{" id=\"10\" "};
    ASSERT_TRUE( xpr.exactMatch( str ) );

    xpr = QRegExp{"\\s+id=\"10\" "};
    ASSERT_TRUE( xpr.exactMatch( str ) );

    xpr = QRegExp{"\\s+id=\"10\"\\s+"};
    ASSERT_TRUE( xpr.exactMatch( str ) );

    xpr = QRegExp{"\\s+\\bid\\b\\s*=\"10\"\\s+"};
    ASSERT_TRUE( xpr.exactMatch( str ) );

    xpr = QRegExp{
        "\\s+\\bid\\b\\s*=\\s*('|\")\\b10\\b('|\")\\s*"}; // !brackets are necessary
    ASSERT_TRUE( xpr.exactMatch( str ) );
    QString str1 = {"\t id=  '10'"};
    ASSERT_TRUE( xpr.exactMatch( str ) );
}

TEST( RegularExpressionTest, EscapeOr ) {
    const QString str1{"\""};
    const QString str2{"'"};
    QRegExp xpr{"\"|'"};
    ASSERT_TRUE( xpr.exactMatch( str1 ) );
}

TEST( RegularExpressionTest, TagTest ) {
    std::string rstr{"<eventitem eventid=\"A335\">"};
    QString str = QString::fromUtf8( rstr.c_str( ) );
    QRegExp xpr{"<eventitem\\b\\s+\\beventid\\b\\s*=\\s*('|\")\\bA335\\b('|\")>"};
    ASSERT_TRUE( xpr.exactMatch( str ) );

    str = QString{"<test id=\"color blue\" class=\"nasty\">"};
    xpr = QRegExp{
        "<test\\b\\s+\\bid\\b\\s*=\\s*('|\")\\bcolor "
        "blue\\b('|\")\\s+\\bclass\\b\\s*('|\")\\bnasty\\b('\")>"};
}
