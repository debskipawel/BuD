#include <iostream>
#include <filesystem>

#include <Serializer.h>

int main()
{
    MG1::SceneSerializer serializer;
    serializer.LoadScene("./file_example_2.json");

    auto& scene = MG1::Scene::Get();

    std::cout << "jak to sie szybko liczy\n";

    serializer.SaveScene("./dupa.json");
    serializer.LoadScene("./dupa.json");
}
