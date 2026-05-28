#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TAGS 100
#define MAX_TAG_LEN 50
#define MAX_FILE_SIZE 5000

// XML Tag များကို သိမ်းဆည်းရန်အတွက် Structure
struct XmlValidator {
    char tags[MAX_TAGS][MAX_TAG_LEN];
    int top;
};

// Function Declarations
void initValidator(struct XmlValidator *v);
bool pushTag(struct XmlValidator *v, char tag[]);
bool popTag(struct XmlValidator *v, char popped[]);
bool validateXml(char content[]);

int main() {
    // ပုံထဲကအတိုင်း note.xml ဖိုင်ကို ဖွင့်လှစ်ဖတ်ရှုခြင်း
    FILE *file = fopen("note.xml", "r");
    if (file == NULL) {
        printf("Error: Could not open note.xml file!\n");
        return 1;
    }

    char fileContent[MAX_FILE_SIZE] = "";
    char line[256];

    // ဖိုင်ထဲက စာသားအားလုံးကို string တစ်ခုထဲသို့ စုစည်းခြင်း
    while (fgets(line, sizeof(line), file)) {
        strcat(fileContent, line);
    }
    fclose(file);

    // XML ကို စစ်ဆေးပြီး ပုံထဲကအတိုင်း Output ထုတ်ခြင်း
    if (validateXml(fileContent)) {
        printf("XML is valid\n");
    } else {
        printf("XML is invalid\n");
    }

    return 0;
}

// Validator Stack ကို အစပြုခြင်း
void initValidator(struct XmlValidator *v) {
    v->top = -1;
}

// အဖွင့် Tag ကို သိမ်းဆည်းခြင်း (Push)
bool pushTag(struct XmlValidator *v, char tag[]) {
    if (v->top >= MAX_TAGS - 1) return false;
    v->top++;
    strcpy(v->tags[v->top], tag);
    return true;
}

// အပိတ် Tag တွေ့ပါက နောက်ဆုံးသိမ်းထားသော အဖွင့် Tag ကို ထုတ်ယူခြင်း (Pop)
bool popTag(struct XmlValidator *v, char popped[]) {
    if (v->top < 0) return false;
    strcpy(popped, v->tags[v->top]);
    v->top--;
    return true;
}

// XML Syntax ကို အဖွင့်အပိတ် စစ်ဆေးပေးသည့် Logic Function
bool validateXml(char content[]) {
    struct XmlValidator validator;
    initValidator(&validator);

    int i = 0;
    int len = strlen(content);

    while (i < len) {
        // Tag တစ်ခု စတင်ကြောင်း သိရှိခြင်း '<'
        if (content[i] == '<') {
            i++;
            // Comment သို့မဟုတ် XML Declaration (<?xml ... ?>) များကို ကျော်ဖြတ်ခြင်း
            if (content[i] == '?' || content[i] == '!') {
                while (i < len && content[i] != '>') i++;
                i++;
                continue;
            }

            bool isClosing = false;
            if (content[i] == '/') {
                isClosing = true;
                i++;
            }

            char currentTag[MAX_TAG_LEN] = "";
            int tagIdx = 0;

            // Tag ရဲ့ နာမည်ကို ဆွဲထုတ်ခြင်း (ဥပမာ- 'note', 'to')
            while (i < len && content[i] != '>' && content[i] != ' ' && content[i] != '/') {
                if (tagIdx < MAX_TAG_LEN - 1) {
                    currentTag[tagIdx++] = content[i];
                }
                i++;
            }
            currentTag[tagIdx] = '\0';

            // Tag အပိတ် '>' မရောက်မချင်း ကျန်တာများကို ကျော်ခြင်း (Attributes များရှိပါက)
            while (i < len && content[i] != '>') {
                i++;
            }
            i++; // '>' ကို ကျော်ရန်

            if (strlen(currentTag) == 0) continue;

            if (isClosing) {
                // အပိတ် tag ဖြစ်ပါက Stack ပေါ်က tag နှင့် တိုက်စစ်ခြင်း
                char lastTag[MAX_TAG_LEN];
                if (!popTag(&validator, lastTag)) {
                    return false; // အဖွင့်မရှိဘဲ အပိတ်လာသဖြင့် invalid ဖြစ်ခြင်း
                }
                if (strcmp(lastTag, currentTag) != 0) {
                    return false; // အဖွင့်နှင့် အပိတ် tag နာမည် မကိုင်ညီသဖြင့် invalid ဖြစ်ခြင်း
                }
            } else {
                // အဖွင့် tag ဖြစ်ပါက Stack ထဲသို့ ထည့်ခြင်း
                if (!pushTag(&validator, currentTag)) {
                    return false;
                }
            }
        } else {
            i++;
        }
    }

    // Stack ထဲမှာ ဘာ tag မှ ကျန်မနေခဲ့မှ အဖွင့်အပိတ်အားလုံး စုံလင်စွာ ကိုက်ညီပြီး Valid ဖြစ်မည်
    return validator.top == -1;
}