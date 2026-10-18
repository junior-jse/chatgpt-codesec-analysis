#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_WORD_LEN 256
#define MAX_SENTENCE_LEN 4096

static bool is_adverb(const char *word, size_t word_len)
{
    static const char *exceptions[] = {
        "very", "too", "quite", "rather", "almost", "nearly", "just",
        "only", "even", "also", "not", "never", "always", "often",
        "sometimes", "seldom", "rarely", "hardly", "scarcely", "barely",
        "soon", "now", "then", "here", "there", "where", "when", "why",
        "how", "well", "fast", "hard", "late", "early", "daily",
        "weekly", "monthly", "yearly", "yesterday", "today", "tomorrow",
        "inside", "outside", "upstairs", "downstairs", "everywhere",
        "anywhere", "somewhere", "nowhere", "away", "back", "home",
        "abroad", "ahead", "behind", "below", "above", "underground",
        "afterwards", "beforehand", "meanwhile", "otherwise", "instead",
        "therefore", "however", "moreover", "furthermore", "nevertheless",
        "nonetheless", "besides", "accordingly", "consequently", "hence",
        "thus", "indeed", "certainly", "surely", "perhaps", "maybe",
        "possibly", "probably", "definitely", "absolutely", "exactly",
        "precisely", "actually", "really", "truly", "literally",
        "virtually", "practically", "basically", "essentially",
        "generally", "usually", "normally", "typically", "mainly",
        "mostly", "largely", "partly", "partially", "completely",
        "totally", "entirely", "fully", "thoroughly", "perfectly",
        "utterly", "extremely", "highly", "deeply", "strongly",
        "closely", "clearly", "obviously", "apparently", "evidently",
        "fortunately", "unfortunately", "luckily", "happily", "sadly",
        "surprisingly", "amazingly", "interestingly", "importantly",
        "significantly", "naturally", "eventually", "finally", "lately",
        "recently", "immediately", "instantly", "suddenly", "gradually",
        "slowly", "quickly", "rapidly", "swiftly", "carefully",
        "carelessly", "easily", "simply", "merely", "purely", "solely",
        "directly", "indirectly", "straight", "right", "wrong", "far",
        "near", "deep", "high", "low", "long", "short", "wide",
        "free", "pretty", "enough", "still", "yet", "already",
        "anymore", "altogether", "apart", "aside", "forward", "backwards",
        "forwards", "afterward", "upward", "downward", "inward",
        "outward", "onward", "henceforth", "thenceforth", "heretofore",
        "hitherto", "forthwith", "wherewith", "thereby", "hereby",
        "whereby", "therein", "herein", "wherein", "thereof", "hereof",
        "whereof", "thereto", "hereto", "whereto", "thereupon",
        "hereupon", "whereupon", "therewith", "herewith", "elsewhere",
        "else", "ever", "once", "twice", "thrice", "seldomly",
        "whither", "whence", "hither", "thither", "yonder", "aloft",
        "afar", "ashore", "aground", "adrift", "afloat", "asleep",
        "awake", "alive", "alone", "aloud", "apart", "around",
        "askew", "astray", "asunder", "awry", "anyway", "anyways",
        "anyhow", "somewhat", "anywise", "nowise", "lengthwise",
        "crosswise", "clockwise", "counterclockwise", "likewise",
        "otherwise", "sideways", "edgewise", "endwise", "flatwise",
        "slantwise", "coastwise", "leastwise", "piecewise", "stepwise",
        "streetwise", "timewise", "weatherwise", "widthwise", "worldlywise",
        "crabwise", "dropwise", "foldwise", "frogwise", "gadzooks",
        "leastways", "lengthways", "sideling", "sideling", "slaunchwise",
        "sunwise", "thenceforward", "thenceforwards", "whitherward",
        "whitherwards", "widdershins", "withershins", "yestereve",
        "yestermorn", "yesternight", "yesterweek", "yesteryear",
        "forsooth", "peradventure", "perchance", "mayhap", "belike",
        "certes", "forwhy", "howbeit", "albeit", "notwithstanding",
        "howsoever", "whatsoever", "wheresoever", "whensoever",
        "whithersoever", "whosoever", "whomsoever", "whosesoever",
        "whatsoever", "whencesoever", "whereabout", "whereabouts",
        "whereafter", "whereas", "whereat", "wherefore", "wherefrom",
        "whereinto", "whereof", "whereon", "wheresoever", "wherethrough",
        "wheretill", "whereto", "whereunder", "whereuntil", "whereunto",
        "whereup", "whereupon", "wherewith", "wherewithal", "hereabout",
        "hereabouts", "hereafter", "hereafters", "hereat", "herebefore",
        "hereinabove", "hereinafter", "hereinbefore", "hereinbelow",
        "hereinto", "heretofore", "hereunder", "hereunto", "hereup",
        "hereupon", "herewith", "thereabout", "thereabouts", "thereafter",
        "thereafters", "thereagainst", "thereamong", "thereamongst",
        "thereat", "therebefore", "therebeneath", "thereby", "therefor",
        "therefore", "therefrom", "therein", "thereinafter", "thereinbefore",
        "thereinto", "thereof", "thereon", "thereout", "thereover",
        "therethrough", "theretill", "thereto", "theretofore", "thereunder",
        "thereuntil", "thereunto", "thereup", "thereupon", "therewith",
        "therewithal", "therewithout", "hitherto", "hitherward",
        "hitherwards", "thitherward", "thitherwards", "whitherward",
        "whitherwards", "whenceforth", "whenceforward", "whenceforwards",
        "whencesoever", "whereever", "wherever", "whenever", "whatever",
        "whichever", "whoever", "whomever", "whosoever", "whomsoever",
        "however", "howsoever", "whyever", "whatsoever", "whosesoever",
        "wheresoever", "whensoever", "whithersoever", "whencesoever",
        "whereabout", "whereabouts", "whereafter", "whereas", "whereat",
        "wherefore", "wherefrom", "wherein", "whereinto", "whereof",
        "whereon", "wheresoever", "wherethrough", "wheretill", "whereto",
        "whereunder", "whereuntil", "whereunto", "whereup", "whereupon",
        "wherever", "wherewith", "wherewithal", "hereabout", "hereabouts",
        "hereafter", "hereafters", "hereat", "herebefore", "hereby",
        "herein", "hereinabove", "hereinafter", "hereinbefore",
        "hereinbelow", "hereinto", "hereof", "hereon", "hereto",
        "heretofore", "hereunder", "hereunto", "hereup", "hereupon",
        "herewith", "thereabout", "thereabouts", "thereafter",
        "thereafters", "thereagainst", "thereamong", "thereamongst",
        "thereat", "therebefore", "therebeneath", "thereby", "therefor",
        "therefore", "therefrom", "therein", "thereinafter",
        "thereinbefore", "thereinto", "thereof", "thereon", "thereout",
        "thereover", "therethrough", "theretill", "thereto",
        "theretofore", "thereunder", "thereuntil", "thereunto",
        "thereup", "thereupon", "therewith", "therewithal",
        "therewithout", "hitherto", "hitherward", "hitherwards",
        "thither", "thitherward", "thitherwards", "whither",
        "whithersoever", "whitherward", "whitherwards", "whence",
        "whenceforth", "whenceforward", "whenceforwards", "whencesoever",
        "whereever", "wherever", "whenever", "whatever", "whatsoever",
        "whencesoever", "whenever", "whensoever", "whereever",
        "wheresoever", "wherever", "whichever", "whichsoever",
        "whichever", "whoever", "whomever", "whomsoever", "whosever",
        "whosesoever", "whosever", "whoso", "whosoever", "whyever",
        "whysoever", "howbeit", "however", "howsoever", "albeit",
        "notwithstanding", "forasmuch", "inasmuch", "insomuch",
        "forwhy", "forsooth", "peradventure", "perchance", "mayhap",
        "mayhaps", "belike", "certes", "pardie", "perdie", "gramercy",
        "marry", "zounds", "gadzooks", "egad", "odds", "sblood",
        "zooks", "tush", "fie", "pish", "pshaw", "bah", "humbug",
        "fudge", "bosh", "rot", "nonsense", "rubbish", "poppycock",
        "balderdash", "tommyrot", "hogwash", "hooey", "malarkey",
        "blarney", "flapdoodle", "twaddle", "drivel", "gibberish",
        "jabberwocky", "gobbledygook", "doubletalk", "bafflegab",
        "legalese", "journalese", "officialese", "computerese",
        "bureaucratese", "educationese", "medicalese", "legalese",
        "sociologese", "psychologese", "economese", "politicalese",
        "militarese", "navalese", "aeronese", "astronautese",
        "technobabble", "psychobabble", "biobabble", "ecobabble",
        "eurobabble", "adverbially", "adverb", NULL
    };
    
    if (word == NULL || word_len == 0 || word_len >= MAX_WORD_LEN) {
        return false;
    }
    
    size_t i;
    for (i = 0; exceptions[i] != NULL; i++) {
        size_t exception_len = strlen(exceptions[i]);
        if (exception_len == word_len && memcmp(word, exceptions[i], word_len) == 0) {
            return true;
        }
    }
    
    if (word_len > 2 && word[word_len - 2] == 'l' && word[word_len - 1] == 'y') {
        return true;
    }
    
    return false;
}

static void find_adverbs(const char *sentence, size_t sentence_len)
{
    if (sentence == NULL || sentence_len == 0 || sentence_len >= MAX_SENTENCE_LEN) {
        fprintf(stderr, "Error: invalid sentence\n");
        return;
    }
    
    char *buffer = malloc(sentence_len + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return;
    }
    
    char *word = malloc(MAX_WORD_LEN);
    if (word == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        free(buffer);
        return;
    }
    
    size_t i = 0;
    int found = 0;
    
    memcpy(buffer, sentence, sentence_len);
    buffer[sentence_len] = '\0';
    
    while (i < sentence_len && buffer[i] != '\0') {
        if (isalpha((unsigned char)buffer[i])) {
            size_t word_start = i;
            size_t word_len = 0;
            
            while (i < sentence_len && isalpha((unsigned char)buffer[i]) && word_len < MAX_WORD_LEN - 1) {
                word[word_len] = (char)tolower((unsigned char)buffer[i]);
                word_len++;
                i++;
            }
            word[word_len] = '\0';
            
            if (word_len > 0 && is_adverb(word, word_len)) {
                printf("Adverb: %-20s Position: %zu\n", word, word_start);
                found = 1;
            }
        } else {
            i++;
        }
    }
    
    if (!found) {
        printf("No adverbs found.\n");
    }
    
    free(word);
    free(buffer);
}

int main(void)
{
    char *sentence = malloc(MAX_SENTENCE_LEN);
    if (sentence == NULL) {
        fprintf(stderr, "Error: memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("Enter a sentence: ");
    
    if (fgets(sentence, MAX_SENTENCE_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(sentence);
        return EXIT_FAILURE;
    }
    
    size_t len = strcspn(sentence, "\n");
    sentence[len] = '\0';
    
    if (len == 0) {
        fprintf(stderr, "Error: empty input\n");
        free(sentence);
        return EXIT_FAILURE;
    }
    
    if (len >= MAX_SENTENCE_LEN) {
        fprintf(stderr, "Error: sentence too long\n");
        free(sentence);
        return EXIT_FAILURE;
    }
    
    find_adverbs(sentence, len);
    
    free(sentence);
    return EXIT_SUCCESS;
}