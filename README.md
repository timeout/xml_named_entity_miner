# xml_named_entity_miner
This is a university project.

The application is for annotating named entities in semi-structured text (XML)

[XML Named Entity Miner screenshot](http://i.imgur.com/T5qtVCh.png)

Example:
from:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<article>
  [...]
<subheading>Konkurrenten und Feinde</subheading>
    <paragraph>Merkels Erfolgsrezept ist der Erfolg – solange die CDU unter ihr Wahlen gewinnt, müssen die Konkurrenten
        stillhalten. Am schwersten fällt es dem hessischen Ministerpräsidenten Roland Koch, seinen Drang an die Spitze
        zu kaschieren. Bayerns Landesvater Edmund Stoiber war bereits Kanzlerkandidat, hatte also seine Chance. Das
        heisst aber nicht, dass er Merkel nun rückhaltlos unterstützt. Gefährlich könnte Christian Wulff werden, der
        Regierungschef Niedersachsens. Regelmässig wird er bei Umfragen zum beliebtesten Politiker gekürt. Zudem kann er
        seinen Ehrgeiz zügeln. Koch und Wulff gehören zum «Andenpakt», den Jungpolitiker vor 25 Jahren auf einer
        Südamerika-Reise gegründet haben. Viele Paktmitglieder, allesamt Männer, haben sich in der Partei über
        Jahrzehnte hochgedient – das unterscheidet sie von der ostdeutschen Senkrechtstarterin Merkel.
    </paragraph>
    <paragraph>Regelrechte Feinde Merkels sind zwei Männer: ihre Vorgänger als Partei- beziehungsweise als
        Fraktionschef, Wolfgang Schäuble und Friedrich Merz. Beide sind fachlich hoch geachtet, beide hat Merkel zur
        Seite geschoben. Schäuble gilt als verbittert. Merz, den sie rabiat aus dem Amt drängte, hat sich zurückgezogen
        und lauert im Hintergrund auf eine neue Chance.
    </paragraph>
  </section>
  [...]
</article>
```

to:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<article>
  <title>Angela Merkel</title>
  [...]
  <section>
    <subheading>Konkurrenten und Feinde</subheading>
    <paragraph><entity type="Persons" synonym="Angela Merkel">Merkel</entity>s Erfolgsrezept ist der Erfolg – solange die <entity type="Organisations">CDU</entity> unter ihr Wahlen gewinnt, müssen die Konkurrenten
        stillhalten. Am schwersten fällt es dem hessischen Ministerpräsidenten <entity type="Persons">Roland Koch</entity>, seinen Drang an die Spitze
        zu kaschieren. Bayerns Landesvater <entity type="Persons">Edmund Stoiber</entity> war bereits Kanzlerkandidat, hatte also seine Chance. Das
        heisst aber nicht, dass er <entity type="Persons" synonym="Angela Merkel">Merkel</entity> nun rückhaltlos unterstützt. Gefährlich könnte <entity type="Persons">Christian Wulff</entity> werden, der
        Regierungschef Niedersachsens. Regelmässig wird er bei Umfragen zum beliebtesten Politiker gekürt. Zudem kann er
        seinen Ehrgeiz zügeln. <entity type="Persons" synonym="Roland Koch">Koch</entity> und <entity type="Persons" synonym="Christian Wulff">Wulff</entity> gehören zum «Andenpakt», den Jungpolitiker vor 25 Jahren auf einer
        Südamerika-Reise gegründet haben. Viele Paktmitglieder, allesamt Männer, haben sich in der Partei über
        Jahrzehnte hochgedient – das unterscheidet sie von der ostdeutschen Senkrechtstarterin <entity type="Persons" synonym="Angela Merkel">Merkel</entity>.
    </paragraph>
    <paragraph>Regelrechte Feinde <entity type="Persons" synonym="Angela Merkel">Merkel</entity>s sind zwei Männer: ihre Vorgänger als Partei- beziehungsweise als
        Fraktionschef, <entity type="Persons">Wolfgang Schäuble</entity> und <entity type="Persons">Friedrich Merz</entity>. Beide sind fachlich hoch geachtet, beide hat <entity type="Persons" synonym="Angela Merkel">Merkel</entity> zur
        Seite geschoben. <entity type="Persons" synonym="Wolfgang Sch&#xE4;uble">Schäuble</entity> gilt als verbittert. <entity type="Persons" synonym="Friedrich Merz">Merz</entity>, den sie rabiat aus dem Amt drängte, hat sich zurückgezogen
        und lauert im Hintergrund auf eine neue Chance.
    </paragraph>
  </section>
  [...]
</article>
```
