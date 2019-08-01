package my.play;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Field;
import java.nio.file.FileSystem;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.MessageFormat;
import java.util.Arrays;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class UDFs {

    public static native byte[] run(KafkaMessage msg) throws Exception;

    private final static String ENV_JNI_DLL_DIR = "JNI_DLL_DIR";

    private final static String dllName = "play_jni";
    private static String zipFileName = "play_jni";
    private static String zipFileExtension = ".zip";
    private static FileSystem fileSystem = FileSystems.getDefault();


    static {

        String currentDir = getCurrentDir();

        log("CurrentDir " + currentDir);

        try {

            String resFolder = extractRes(currentDir, zipFileName, zipFileExtension);
            log("Use ResFolder " + resFolder);

            addLibraryPath(resFolder);
            System.loadLibrary(dllName);

        } catch (Exception exception) {
            throw new RuntimeException(exception);
        }
    }

    private static String getCurrentDir() {
        String currentDir = UDFs.class.getProtectionDomain().getCodeSource().getLocation().toString().replace("file:/", "");

        File f = new File(currentDir);
        // .jar file
        if (f.isFile() && currentDir.endsWith(".jar")) {
            currentDir = f.getParent();
        }

        return currentDir;
    }

    /**
     * @param folder
     * @param zipFileName
     * @param zipExt
     * @return
     * @throws IOException
     */
    private static String extractRes(String folder, String zipFileName, String zipExt) throws IOException {

        String env = System.getenv(ENV_JNI_DLL_DIR);
        if (env != null && !env.isEmpty()) {
            return env;
        }

        File zip = Paths.get(folder, zipFileName + zipExt).toFile();

        // check xxx/MappingInSparkWorker.zip
        // if it is directory, return
        if (zip.exists() && zip.isDirectory()) {
            return zip.toString();
        }

        File dir = Paths.get(folder, zipFileName).toFile();

        // check xxx/MappingInSparkWorker
        if (dir.exists() && dir.isDirectory()) {
            return dir.toString();
        }

        // check xxx/MappingInSparkWorker.zip
        // if it is zip file, extract
        if (zip.exists() && zip.isFile()) {
            unzip(zip.toString(), dir.toString());
            return dir.toString();
        }

        String msg = MessageFormat.format("Cannot find folder [{0}] or [{1}] or zip file [{2}] under [{3}]", zipFileName + zipExt, zipFileName, zipFileName + zipExt, folder);
        log(msg);
        throw new FileNotFoundException(msg);
    }

    private static void addLibraryPath(String pathToAdd) throws Exception {
        final Field usrPathsField = ClassLoader.class.getDeclaredField("usr_paths");
        usrPathsField.setAccessible(true);

        //get array of paths
        final String[] paths = (String[]) usrPathsField.get(null);

        //check if the path to add is already present
        for (String path : paths) {
            if (path.equals(pathToAdd)) {
                return;
            }
        }

        //add the new path
        final String[] newPaths = Arrays.copyOf(paths, paths.length + 1);
        newPaths[newPaths.length - 1] = pathToAdd;
        usrPathsField.set(null, newPaths);

        for (String p : newPaths) {
            //log("include path " + p);
        }
    }

    public static void unzip(String zipFile, String targetDir) throws IOException {

        log("unzip file" + zipFile);
        ZipInputStream zip = new ZipInputStream(new FileInputStream(zipFile));

        Path f = fileSystem.getPath(targetDir);
        if (!Files.exists(f)) Files.createDirectory(f);

        ZipEntry entry;
        while ((entry = zip.getNextEntry()) != null) {

            f = fileSystem.getPath(targetDir, entry.getName());

            if (entry.isDirectory()) {
                log("Create Dir " + f.toString());
                if (!Files.exists(f)) Files.createDirectory(f);
                continue;
            }

            log("Create file " + f.toString());

            if (!Files.exists(f.getParent())) Files.createDirectory(f.getParent());

            FileOutputStream fout = new FileOutputStream(f.toFile());
            BufferedOutputStream bout = new BufferedOutputStream(fout);
            byte[] buffer = new byte[1024];
            int read;
            while ((read = zip.read(buffer)) != -1) {
                bout.write(buffer, 0, read);
            }

            bout.close();
            zip.closeEntry();
            fout.close();
        }

        zip.close();

    }


    private static void log(String msg) {
        System.out.println("NativeLoader:" + msg);
    }
}
